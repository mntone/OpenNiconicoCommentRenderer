#pragma once
#include "DirectXInitializedHandler.h"
#include "DirectXRenderingEventArgs.h"

#pragma warning( disable: 4486 )

namespace Mntone { namespace Nico { namespace Renderer { namespace DirectX {

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DirectXPanelBase
		: public ::Windows::UI::Xaml::Controls::SwapChainPanel
	{
	protected private:
		DirectXPanelBase( DXGI_ALPHA_MODE alphaMode );

	public:
		virtual ~DirectXPanelBase();

	protected:
		void Initialize();

		virtual void OnDeviceIndependentResourcesInitialized();
		virtual void OnDeviceResourcesInitialized();
		virtual void OnSizeDependentResourcesInitialized();
		virtual void OnRendering( DirectXRenderingEventArgs^ e );

	protected private:
		void Present();

		virtual void OnSuspending( ::Platform::Object^ sender, ::Windows::ApplicationModel::SuspendingEventArgs^ e );
		virtual void OnResuming( ::Platform::Object^ sender, ::Platform::Object^ args );
		virtual void OnSizeChanged( ::Platform::Object^ sender, ::Windows::UI::Xaml::SizeChangedEventArgs^ e );
		virtual void OnCompositionScaleChanged( ::Windows::UI::Xaml::Controls::SwapChainPanel^ sender, ::Platform::Object^ args );
		virtual void OnDpiChanged( ::Windows::Graphics::Display::DisplayInformation^ sender, ::Platform::Object^ args );
		virtual void OnDeviceLost();

	private:
		void InitializeDeviceIndependentResources();
		void InitializeDeviceResources();
		void InitializeSizeDependentResources();

		void ReleaseEventHandlers();
		void ReleaseDeviceIndependentResources();
		void ReleaseDeviceResources();
		void ReleaseSizeDependentResources();

		virtual void OnDisplayContentsInvalidated( ::Windows::Graphics::Display::DisplayInformation^ sender, ::Platform::Object^ args );

	private:
		event DirectXInitializedHandler^ DeviceIndependentResourcesInitialized;
		event DirectXInitializedHandler^ DeviceResourcesInitialized;
		event DirectXInitializedHandler^ SizeDependentResourcesInitialized;

	protected private:
		bool initialized_;
		DXGI_ALPHA_MODE alphaMode_;
		float32 Height_, Width_, ScaleY_, ScaleX_, Dpi_;
		::std::mutex mutex_;

		::Microsoft::WRL::ComPtr<ID3D11Device2> d3dDevice_;
		::Microsoft::WRL::ComPtr<ID3D11DeviceContext2> d3dDeviceContext_;

		::Microsoft::WRL::ComPtr<ID2D1Factory2> d2dFactory_;
		::Microsoft::WRL::ComPtr<ID2D1Device1> d2dDevice_;
		::Microsoft::WRL::ComPtr<ID2D1DeviceContext1> d2dDeviceContext_;

		::Microsoft::WRL::ComPtr<IDWriteFactory2> dwriteFactory_;

		::Microsoft::WRL::ComPtr<IDXGISwapChain2> dxgiSwapChain_;
		::Microsoft::WRL::ComPtr<ID2D1Bitmap1> d2dTargetBitmap_;

	private:
		::Windows::Foundation::EventRegistrationToken
			suspendingEventToken_,
			resumingEventToken_,
			sizeChangedEventToken_,
			compositionScaleChangedEventToken_,
			dpiChangedEventToken_,
			displayContentsInvalidatedEventToken_,
			deviceIndpendentResourcesInitializedEventToken_,
			deviceResourcesInitializedEventToken_,
			sizeResourcesInitializedEventToken_;
	};

} } } }