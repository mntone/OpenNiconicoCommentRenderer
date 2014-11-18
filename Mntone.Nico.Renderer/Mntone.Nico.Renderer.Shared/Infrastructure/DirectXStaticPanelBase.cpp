#include "pch.h"
#include "DirectXStaticPanelBase.h"
#include "DirectXHelper.h"

using namespace Platform;
using namespace std;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Mntone::Nico::Renderer::DirectX;

#define LOCK lock_guard<mutex> locker( mutex_ );

DirectXStaticPanelBase::DirectXStaticPanelBase( DXGI_ALPHA_MODE alphaMode )
	: DirectXPanelBase( alphaMode )
{
	renderingEventToken_ = Rendering += ref new DirectXRenderingHandler( this, &DirectXStaticPanelBase::OnRendering );
}

DirectXStaticPanelBase::~DirectXStaticPanelBase()
{
	Rendering -= renderingEventToken_;
}

void DirectXStaticPanelBase::Render()
{
	if( !initialized_ && !IsEnabled )
	{
		return;
	}

	Rendering( ref new DirectXRenderingEventArgs( ++totalFrames_ ) );
	Present();
}

void DirectXStaticPanelBase::OnResuming( Object^ sender, Object^ args )
{
	DirectXPanelBase::OnResuming( sender, args );

	LOCK;
	Render();
}

void DirectXStaticPanelBase::OnSizeChanged( Object^ sender, SizeChangedEventArgs^ e )
{
	DirectXPanelBase::OnSizeChanged( sender, e );

	LOCK;
	Render();
}

void DirectXStaticPanelBase::OnCompositionScaleChanged( SwapChainPanel^ sender, Object^ args )
{
	DirectXPanelBase::OnCompositionScaleChanged( sender, args );

	LOCK;
	Render();
}

void DirectXStaticPanelBase::OnDeviceLost()
{
	DirectXPanelBase::OnDeviceLost();

	// Do not LOCK!
	Render();
}

bool DirectXStaticPanelBase::IsEnabled::get()
{
	auto ret = safe_cast<IBox<bool>^>( GetValue( IsEnabledProperty_ ) );
	return ret ? ret->Value : false;
}

void DirectXStaticPanelBase::IsEnabled::set( bool value )
{
	SetValue( IsEnabledProperty_, value );
}

DependencyProperty^ DirectXStaticPanelBase::IsEnabledProperty_ = DependencyProperty::Register(
	"IsEnabled",
	bool::typeid,
	DirectXStaticPanelBase::typeid,
	PropertyMetadata::Create( true, ref new PropertyChangedCallback( &DirectXStaticPanelBase::OnIsEnabledChanged ) ) );

void DirectXStaticPanelBase::OnIsEnabledChanged( DependencyObject^ d, DependencyPropertyChangedEventArgs^ e )
{
	auto that = safe_cast<DirectXStaticPanelBase^>( d );
	that->IsEnabledChanged( d, e );
}