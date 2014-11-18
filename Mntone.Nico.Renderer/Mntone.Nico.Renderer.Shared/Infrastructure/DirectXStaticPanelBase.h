#pragma once
#include "DirectXPanelBase.h"
#include "DirectXRenderingHandler.h"

namespace Mntone { namespace Nico { namespace Renderer { namespace DirectX {

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DirectXStaticPanelBase
		: public DirectXPanelBase
	{
	protected private:
		DirectXStaticPanelBase( DXGI_ALPHA_MODE alphaMode );

	public:
		virtual ~DirectXStaticPanelBase();

	protected private:
		virtual void OnResuming( ::Platform::Object^ sender, ::Platform::Object^ args ) override sealed;
		virtual void OnSizeChanged( ::Platform::Object^ sender, ::Windows::UI::Xaml::SizeChangedEventArgs^ e ) override sealed;
		virtual void OnCompositionScaleChanged( ::Windows::UI::Xaml::Controls::SwapChainPanel^ sender, ::Platform::Object^ args ) override sealed;
		virtual void OnDeviceLost() override sealed;

	private:
		void Render();

		static void OnIsEnabledChanged( ::Windows::UI::Xaml::DependencyObject^ d, ::Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ e );

	public:
		event Windows::UI::Xaml::DependencyPropertyChangedEventHandler^ IsEnabledChanged;

	private:
		event DirectXRenderingHandler^ Rendering;

	public:
		property bool IsEnabled
		{
			bool get();
			void set( bool value );
		}

		static property ::Windows::UI::Xaml::DependencyProperty^ IsEnabledProperty
		{
			::Windows::UI::Xaml::DependencyProperty^ get() { return IsEnabledProperty_; }
		}

	private:
		uint64 totalFrames_;
		::Windows::Foundation::EventRegistrationToken renderingEventToken_;

		static ::Windows::UI::Xaml::DependencyProperty^ IsEnabledProperty_;
	};

} } } }