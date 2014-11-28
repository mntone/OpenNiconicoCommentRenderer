#include "pch.h"
#include "DirectXDynamicPanelBase.h"
#include "DirectXHelper.h"

using namespace Microsoft::WRL;
using namespace Platform;
using namespace std;
using namespace Windows::ApplicationModel;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Windows::System::Threading;
using namespace Windows::UI::Xaml;
using namespace Mntone::Nico::Renderer::DirectX;

#define LOCK lock_guard<mutex> locker( mutex_ );

DirectXDynamicPanelBase::DirectXDynamicPanelBase( DXGI_ALPHA_MODE alphaMode )
	: DirectXPanelBase( alphaMode )
	, enabled_( false )
{
	renderingEventToken_ = Rendering += ref new DirectXRenderingHandler( this, &DirectXDynamicPanelBase::OnRendering );
}

DirectXDynamicPanelBase::~DirectXDynamicPanelBase()
{
	Rendering -= renderingEventToken_;
}

void DirectXDynamicPanelBase::Suspend()
{
	LOCK;
	if( enabled_ )
	{
		if( loopWorker_ != nullptr )
		{
			loopWorker_->Cancel();
			loopWorker_ = nullptr;
		}

#if _DEBUG
		OutputDebugStringW( L"Suspended.\n" );
#endif
	}
}

void DirectXDynamicPanelBase::Resume()
{
	LOCK;
	if( enabled_ )
	{
		StartRenderingInternal();

#if _DEBUG
		OutputDebugStringW( L"Resumed.\n" );
#endif
	}
}

void DirectXDynamicPanelBase::StartRendering()
{
	LOCK;
	StartRenderingInternal();
}

void DirectXDynamicPanelBase::StartRenderingInternal()
{
	if( loopWorker_ != nullptr && loopWorker_->Status == AsyncStatus::Started )
	{
		return;
	}

	auto workItemHandler = ref new WorkItemHandler( [this]( IAsyncAction^ action )
	{
		while( action->Status == AsyncStatus::Started )
		{
			timer_.Update( [&]
			{
				Render();
			} );

			dxgiOutput_->WaitForVBlank();
		}
	} );

	timer_.Reset();
	loopWorker_ = ThreadPool::RunAsync( workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced );
	enabled_ = true;
}

void DirectXDynamicPanelBase::EndRendering()
{
	if( loopWorker_ != nullptr )
	{
		loopWorker_->Cancel();
		loopWorker_ = nullptr;
	}
	enabled_ = false;
}

void DirectXDynamicPanelBase::Render()
{
	LOCK;
	if( !initialized_ )
	{
		return;
	}

	Rendering( ref new DirectXRenderingEventArgs( timer_.FramesPerSecond(), timer_.TotalFrames() ) );
	Present();
}

void DirectXDynamicPanelBase::OnSizeDependentResourcesInitialized()
{
	DirectXPanelBase::OnSizeDependentResourcesInitialized();

	ComPtr<IDXGIDevice3> dxgiDevice;
	DirectXHelper::CheckHResult( d3dDevice_.As( &dxgiDevice ) );

	ComPtr<IDXGIAdapter> dxgiAdapter;
	DirectXHelper::CheckHResult( dxgiDevice->GetAdapter( dxgiAdapter.GetAddressOf() ) );

	ComPtr<IDXGIOutput> dxgiOutput;
	DirectXHelper::CheckHResult( dxgiAdapter->EnumOutputs( 0, dxgiOutput.GetAddressOf() ) );
	DirectXHelper::CheckHResult( dxgiOutput.As( &dxgiOutput_ ) );
}

void DirectXDynamicPanelBase::OnSuspending( Object^ sender, SuspendingEventArgs^ e )
{
	Suspend();

	DirectXDynamicPanelBase::OnSuspending( sender, e );
}

void DirectXDynamicPanelBase::OnResuming( Object^ sender, Object^ args )
{
	Resume();
}

bool DirectXDynamicPanelBase::IsEnabled::get()
{
	auto ret = safe_cast<IBox<bool>^>( GetValue( IsEnabledProperty_ ) );
	return ret ? ret->Value : false;
}

void DirectXDynamicPanelBase::IsEnabled::set( bool value )
{
	SetValue( IsEnabledProperty_, value );
}

DependencyProperty^ DirectXDynamicPanelBase::IsEnabledProperty_ = DependencyProperty::Register(
	"IsEnabled",
	bool::typeid,
	DirectXDynamicPanelBase::typeid,
	PropertyMetadata::Create( true, ref new PropertyChangedCallback( &DirectXDynamicPanelBase::OnIsEnabledChanged ) ) );

void DirectXDynamicPanelBase::OnIsEnabledChanged( DependencyObject^ d, DependencyPropertyChangedEventArgs^ e )
{
	auto that = safe_cast<DirectXDynamicPanelBase^>( d );
	that->IsEnabledChanged( d, e );

	auto newValue = safe_cast<IBox<bool>^>( e->NewValue );
	if( newValue )
	{
		if( newValue->Value )
		{
			that->Resume();
		}
		else
		{
			that->Suspend();
		}
	}
}