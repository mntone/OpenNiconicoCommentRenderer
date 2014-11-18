#include "pch.h"
#include "DirectXPanelBase.h"
#if _DEBUG
#include <dxgidebug.h>
#endif
#include <mutex>
#include "DirectXHelper.h"

using namespace Microsoft::WRL;
using namespace Platform;
using namespace std;
using namespace Windows::ApplicationModel;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Mntone::Nico::Renderer::DirectX;

#define LOCK lock_guard<mutex> locker( mutex_ );

DirectXPanelBase::DirectXPanelBase( DXGI_ALPHA_MODE alphaMode )
	: initialized_( false )
	, alphaMode_( alphaMode )
	, Height_( 1.f )
	, Width_( 1.f )
	, ScaleY_( 1.f )
	, ScaleX_( 1.f )
	, Dpi_( 1.f )
{
	suspendingEventToken_ = Application::Current->Suspending += ref new SuspendingEventHandler( this, &DirectXPanelBase::OnSuspending );
	resumingEventToken_ = Application::Current->Resuming += ref new EventHandler<Object^>( this, &DirectXPanelBase::OnResuming );
	sizeChangedEventToken_ = SizeChanged += ref new SizeChangedEventHandler( this, &DirectXPanelBase::OnSizeChanged );
	compositionScaleChangedEventToken_ = CompositionScaleChanged += ref new TypedEventHandler<SwapChainPanel^, Object^>( this, &DirectXPanelBase::OnCompositionScaleChanged );
	
	auto displayInformation = DisplayInformation::GetForCurrentView();
	dpiChangedEventToken_ = displayInformation->DpiChanged += ref new TypedEventHandler<DisplayInformation^, Object^>( this, &DirectXPanelBase::OnDpiChanged );
	Dpi_ = displayInformation->LogicalDpi;

	displayContentsInvalidatedEventToken_ = DisplayInformation::DisplayContentsInvalidated += ref new TypedEventHandler<DisplayInformation^, Object^>( this, &DirectXPanelBase::OnDisplayContentsInvalidated );

	deviceIndpendentResourcesInitializedEventToken_ = DeviceIndependentResourcesInitialized += ref new DirectXInitializedHandler( this, &DirectXPanelBase::OnDeviceIndependentResourcesInitialized );
	deviceResourcesInitializedEventToken_ = DeviceResourcesInitialized += ref new DirectXInitializedHandler( this, &DirectXPanelBase::OnDeviceResourcesInitialized );
	sizeResourcesInitializedEventToken_ = SizeDependentResourcesInitialized += ref new DirectXInitializedHandler( this, &DirectXPanelBase::OnSizeDependentResourcesInitialized );
}

DirectXPanelBase::~DirectXPanelBase()
{
	LOCK;
	initialized_ = false;

	ReleaseSizeDependentResources();
	ReleaseDeviceResources();
	ReleaseDeviceIndependentResources();
	ReleaseEventHandlers();
}

void DirectXPanelBase::Initialize()
{
	LOCK;
	InitializeDeviceIndependentResources();
	InitializeDeviceResources();
	InitializeSizeDependentResources();
}

void DirectXPanelBase::Present()
{
	DXGI_PRESENT_PARAMETERS parameters = { 0 };
	parameters.DirtyRectsCount = 0;
	parameters.pDirtyRects = nullptr;
	parameters.pScrollRect = nullptr;
	parameters.pScrollOffset = nullptr;

	HRESULT hr = dxgiSwapChain_->Present1( 1, 0, &parameters );
	if( hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET )
	{
		OnDeviceLost();
	}
	else
	{
		DirectXHelper::CheckHResult( hr );
	}
}

#pragma region Resource processing

void DirectXPanelBase::OnDeviceIndependentResourcesInitialized()
{
#if _DEBUG
	OutputDebugStringW( L"DirectXPanelBase::OnDeviceIndependentResourcesInitialized called.\n" );
#endif
}

void DirectXPanelBase::OnDeviceResourcesInitialized()
{
#if _DEBUG
	OutputDebugStringW( L"DirectXPanelBase::OnDeviceResourcesInitialized called.\n" );
#endif
}

void DirectXPanelBase::OnSizeDependentResourcesInitialized()
{
#if _DEBUG
	OutputDebugStringW( L"DirectXPanelBase::OnSizeDependentResourcesInitialized called.\n" );
#endif
}

void DirectXPanelBase::OnRendering( DirectXRenderingEventArgs^ e )
{ }

#pragma endregion


#pragma region Initializer

void DirectXPanelBase::InitializeDeviceIndependentResources()
{
	DirectXHelper::CheckHResult( D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, d2dFactory_.GetAddressOf() ) );
	DirectXHelper::CheckHResult( DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED, __uuidof( IDWriteFactory2 ), &dwriteFactory_ ) );
	DeviceIndependentResourcesInitialized();
}

void DirectXPanelBase::InitializeDeviceResources()
{
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	const D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	ComPtr<ID3D11Device> d3dDevice;
	ComPtr<ID3D11DeviceContext> d3dDeviceContext;
	HRESULT hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		ARRAYSIZE( featureLevels ),
		D3D11_SDK_VERSION,
		d3dDevice.GetAddressOf(),
		nullptr,
		d3dDeviceContext.GetAddressOf() );
	if( FAILED( hr ) )
	{
		DirectXHelper::CheckHResult( D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_WARP,
			nullptr,
			creationFlags,
			featureLevels,
			ARRAYSIZE( featureLevels ),
			D3D11_SDK_VERSION,
			d3dDevice.GetAddressOf(),
			nullptr,
			d3dDeviceContext.GetAddressOf() ) );
	}
	DirectXHelper::CheckHResult( d3dDevice.As( &d3dDevice_ ) );
	DirectXHelper::CheckHResult( d3dDeviceContext.As( &d3dDeviceContext_ ) );

	ComPtr<IDXGIDevice3> dxgiDevice;
	DirectXHelper::CheckHResult( d3dDevice_.As( &dxgiDevice ) );

	DirectXHelper::CheckHResult( dxgiDevice->SetMaximumFrameLatency( 1 ) );

	ComPtr<ID2D1Device> d2dDevice;
	DirectXHelper::CheckHResult( d2dFactory_->CreateDevice( dxgiDevice.Get(), d2dDevice.GetAddressOf() ) );
	DirectXHelper::CheckHResult( d2dDevice.As( &d2dDevice_ ) );

	DirectXHelper::CheckHResult( d2dDevice_->CreateDeviceContext( D2D1_DEVICE_CONTEXT_OPTIONS_NONE, d2dDeviceContext_.GetAddressOf() ) );

	d2dDeviceContext_->SetAntialiasMode( D2D1_ANTIALIAS_MODE_PER_PRIMITIVE );
	d2dDeviceContext_->SetTextAntialiasMode( D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE );

	DeviceResourcesInitialized();
	initialized_ = true;
}

void DirectXPanelBase::InitializeSizeDependentResources()
{
	d2dDeviceContext_->SetTarget( nullptr );
	d2dTargetBitmap_ = nullptr;
	d3dDeviceContext_->OMSetRenderTargets( 0, nullptr, nullptr );
	d3dDeviceContext_->Flush();

	const auto physicalHeight = static_cast<UINT>( ScaleY_ * Height_ );
	const auto physicalWidth = static_cast<UINT>( ScaleX_ * Width_ );
	if( dxgiSwapChain_ != nullptr )
	{
		HRESULT hr = dxgiSwapChain_->ResizeBuffers(
			2,
			physicalWidth,
			physicalHeight,
			DXGI_FORMAT_B8G8R8A8_UNORM,
			0 );
		if( hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET )
		{
			OnDeviceLost();
			return;
		}
		DirectXHelper::CheckHResult( hr );
	}
	else
	{
		ComPtr<IDXGIDevice3> dxgiDevice;
		DirectXHelper::CheckHResult( d3dDevice_.As( &dxgiDevice ) );

		ComPtr<IDXGIAdapter> dxgiAdapter;
		DirectXHelper::CheckHResult( dxgiDevice->GetAdapter( dxgiAdapter.GetAddressOf() ) );

		ComPtr<IDXGIFactory2> dxgiFactory;
		DirectXHelper::CheckHResult( dxgiAdapter->GetParent( IID_PPV_ARGS( dxgiFactory.GetAddressOf() ) ) );

		DXGI_SWAP_CHAIN_DESC1 dxgiSwapChainDesc = { 0 };
		dxgiSwapChainDesc.Width = physicalWidth;
		dxgiSwapChainDesc.Height = physicalHeight;
		dxgiSwapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		dxgiSwapChainDesc.Stereo = false;
		dxgiSwapChainDesc.SampleDesc.Count = 1;
		dxgiSwapChainDesc.SampleDesc.Quality = 0;
		dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		dxgiSwapChainDesc.BufferCount = 2;
		dxgiSwapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		dxgiSwapChainDesc.AlphaMode = alphaMode_;
		dxgiSwapChainDesc.Flags = 0;

		ComPtr<IDXGISwapChain1> dxgiSwapChain;
		DirectXHelper::CheckHResult( dxgiFactory->CreateSwapChainForComposition(
			d3dDevice_.Get(),
			&dxgiSwapChainDesc,
			nullptr,
			dxgiSwapChain.GetAddressOf() ) );
		DirectXHelper::CheckHResult( dxgiSwapChain.As( &dxgiSwapChain_ ) );

		auto func = [this]
		{
			ComPtr<ISwapChainPanelNative> swapChainPanelNative;
			DirectXHelper::CheckHResult( reinterpret_cast<IUnknown*>( this )->QueryInterface( IID_PPV_ARGS( swapChainPanelNative.GetAddressOf() ) ) );
			DirectXHelper::CheckHResult( swapChainPanelNative->SetSwapChain( dxgiSwapChain_.Get() ) );
		};
		if( Dispatcher->HasThreadAccess )
		{
			func();
		}
		else
		{
			Dispatcher->RunAsync( CoreDispatcherPriority::Normal, ref new DispatchedHandler( func, CallbackContext::Any ) );
		}
	}

	DXGI_MATRIX_3X2_F inverseScale = { 0 };
	inverseScale._11 = 1.0f / ScaleX_;
	inverseScale._22 = 1.0f / ScaleY_;
	dxgiSwapChain_->SetMatrixTransform( &inverseScale );

	ComPtr<IDXGISurface> dxgiBackBuffer;
	DirectXHelper::CheckHResult( dxgiSwapChain_->GetBuffer( 0, IID_PPV_ARGS( &dxgiBackBuffer ) ) );

	auto dpiY = ScaleY_ * Dpi_;
	auto dpiX = ScaleX_ * Dpi_;

	D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat( DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED ),
		dpiX, dpiY );
	DirectXHelper::CheckHResult( d2dDeviceContext_->CreateBitmapFromDxgiSurface( dxgiBackBuffer.Get(), &bitmapProperties, d2dTargetBitmap_.GetAddressOf() ) );

	d2dDeviceContext_->SetDpi( dpiX, dpiY );
	d2dDeviceContext_->SetTarget( d2dTargetBitmap_.Get() );

	SizeDependentResourcesInitialized();
}

#pragma endregion


#pragma region Releaser

void DirectXPanelBase::ReleaseEventHandlers()
{
	Application::Current->Suspending -= suspendingEventToken_;
	Application::Current->Resuming -= resumingEventToken_;
	SizeChanged -= sizeChangedEventToken_;
	CompositionScaleChanged -= compositionScaleChangedEventToken_;
	
	auto displayInformation = DisplayInformation::GetForCurrentView();
	displayInformation->DpiChanged -= dpiChangedEventToken_;
	DisplayInformation::DisplayContentsInvalidated -= displayContentsInvalidatedEventToken_;

	DeviceIndependentResourcesInitialized -= deviceIndpendentResourcesInitializedEventToken_;
	DeviceResourcesInitialized -= deviceResourcesInitializedEventToken_;
	SizeDependentResourcesInitialized -= sizeResourcesInitializedEventToken_;
}

void DirectXPanelBase::ReleaseDeviceIndependentResources()
{
	dwriteFactory_ = nullptr;
	d2dFactory_ = nullptr;
}

void DirectXPanelBase::ReleaseDeviceResources()
{
	d2dDeviceContext_ = nullptr;
	d2dDevice_ = nullptr;

	d3dDeviceContext_ = nullptr;
	d3dDevice_ = nullptr;
}

void DirectXPanelBase::ReleaseSizeDependentResources()
{
	d2dDeviceContext_->SetTarget( nullptr );
	d2dTargetBitmap_ = nullptr;
	d3dDeviceContext_->OMSetRenderTargets( 0, nullptr, nullptr );
	d3dDeviceContext_->Flush();
	dxgiSwapChain_ = nullptr;
}

#pragma endregion


#pragma region Events

void DirectXPanelBase::OnSuspending( Object^ sender, SuspendingEventArgs^ e )
{
	LOCK;

	ComPtr<IDXGIDevice3> dxgiDevice;
	DirectXHelper::CheckHResult( d3dDevice_.As( &dxgiDevice ) );
	dxgiDevice->Trim();
}

void DirectXPanelBase::OnResuming( Object^ sender, Object^ args )
{ }

void DirectXPanelBase::OnSizeChanged( Object^ sender, SizeChangedEventArgs^ e )
{
#if _DEBUG
	OutputDebugStringW( L"DirectXPanelBase::OnSizeChanged called.\n" );
#endif

	const auto newHeight = e->NewSize.Height;
	const auto newWidth = e->NewSize.Width;
	if( newHeight != Height_ || newWidth != Width_ )
	{
		LOCK;
		Height_ = max( newHeight, 1.f );
		Width_ = max( newWidth, 1.f );
		InitializeSizeDependentResources();
	}
}

void DirectXPanelBase::OnCompositionScaleChanged( SwapChainPanel^ sender, Object^ args )
{
#if _DEBUG
	OutputDebugStringW( L"DirectXPanelBase::OnCompositionScaleChanged called.\n" );
#endif

	const auto newScaleY = CompositionScaleY;
	const auto newScaleX = CompositionScaleX;
	if( newScaleY != ScaleY_ || newScaleX != ScaleX_ )
	{
		LOCK;
		ScaleY_ = max( newScaleY, 1.f );
		ScaleX_ = max( newScaleX, 1.f );
		InitializeSizeDependentResources();
	}
}

void DirectXPanelBase::OnDpiChanged( DisplayInformation^ sender, Object^ args )
{
#if _DEBUG
	OutputDebugStringW( L"DirectXPanelBase::OnDpiChanged called.\n" );
#endif

	const auto newDpi = sender->LogicalDpi;
	if( newDpi != Dpi_ )
	{
		LOCK;
		Dpi_ = max( newDpi, 1.f );
		InitializeSizeDependentResources();
	}
}

void DirectXPanelBase::OnDisplayContentsInvalidated( DisplayInformation^ sender, Object^ args )
{
#if _DEBUG
	OutputDebugStringW( L"DirectXPanelBase::OnDisplayContentsInvalidated called.\n" );
#endif

	ComPtr<IDXGIDevice3> dxgiDevice;
	DirectXHelper::CheckHResult( d3dDevice_.As( &dxgiDevice ) );

	ComPtr<IDXGIAdapter> dxgiAdapter;
	DirectXHelper::CheckHResult( dxgiDevice->GetAdapter( dxgiAdapter.GetAddressOf() ) );


	ComPtr<IDXGIFactory2> previousDxgiFactory;
	DirectXHelper::CheckHResult( dxgiAdapter->GetParent( IID_PPV_ARGS( previousDxgiFactory.GetAddressOf() ) ) );

	ComPtr<IDXGIAdapter1> previousDxgiAdapter;
	DirectXHelper::CheckHResult( previousDxgiFactory->EnumAdapters1( 0, previousDxgiAdapter.GetAddressOf() ) );

	DXGI_ADAPTER_DESC1 previousDxgiAdapterDesc;
	DirectXHelper::CheckHResult( previousDxgiAdapter->GetDesc1( &previousDxgiAdapterDesc ) );


	UINT creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
	ComPtr<IDXGIFactory2> currentDxgiFactory;
	DirectXHelper::CheckHResult( CreateDXGIFactory2( creationFlags, IID_PPV_ARGS( currentDxgiFactory.GetAddressOf() ) ) );

	ComPtr<IDXGIAdapter1> currentDxgiAdapter;
	DirectXHelper::CheckHResult( currentDxgiFactory->EnumAdapters1( 0, &currentDxgiAdapter ) );

	DXGI_ADAPTER_DESC1 currentDxgiAdapterDesc;
	DirectXHelper::CheckHResult( currentDxgiAdapter->GetDesc1( &currentDxgiAdapterDesc ) );

	if( previousDxgiAdapterDesc.AdapterLuid.HighPart != currentDxgiAdapterDesc.AdapterLuid.HighPart
		|| previousDxgiAdapterDesc.AdapterLuid.LowPart != currentDxgiAdapterDesc.AdapterLuid.LowPart )
	{
		LOCK;
		OnDeviceLost();
	}
}

void DirectXPanelBase::OnDeviceLost()
{
#if _DEBUG
	OutputDebugStringW( L"DirectXPanelBase::OnDeviceLost called.\n" );
#endif

	initialized_ = false;

	ReleaseSizeDependentResources();
	ReleaseDeviceResources();

	InitializeDeviceResources();
	InitializeSizeDependentResources();
}

#pragma endregion