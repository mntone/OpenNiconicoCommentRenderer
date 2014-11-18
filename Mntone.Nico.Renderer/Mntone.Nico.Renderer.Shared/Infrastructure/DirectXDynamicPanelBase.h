#pragma once
#include "DirectXPanelBase.h"
#include "DirectXRenderingHandler.h"
#include "StepTimer.h"

namespace Mntone { namespace Nico { namespace Renderer { namespace DirectX {

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DirectXDynamicPanelBase
		: public DirectXPanelBase
	{
	protected private:
		DirectXDynamicPanelBase( DXGI_ALPHA_MODE alphaMode );

	public:
		virtual ~DirectXDynamicPanelBase();

		void StartRendering();
		void EndRendering();

	protected:
		virtual void OnSizeDependentResourcesInitialized() override;

	protected private:
		virtual void OnSuspending( ::Platform::Object^ sender, ::Windows::ApplicationModel::SuspendingEventArgs^ e ) override sealed;
		virtual void OnResuming( ::Platform::Object^ sender, ::Platform::Object^ args ) override sealed;

	private:
		void Render();

		void StartRenderingInternal();
		void Suspend();
		void Resume();

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
		bool enabled_;
		StepTimer timer_;
		::Windows::Foundation::EventRegistrationToken renderingEventToken_;
		::Windows::Foundation::IAsyncAction^ loopWorker_;
		::Microsoft::WRL::ComPtr<IDXGIOutput2> dxgiOutput_;

		static ::Windows::UI::Xaml::DependencyProperty^ IsEnabledProperty_;
	};

} } } }