#pragma once
#include "App.g.h"

namespace Mntone { namespace Nico { namespace Renderer { namespace Demo {

	ref class App sealed
	{
	public:
		App();

		virtual void OnLaunched( ::Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e ) override sealed;

	private:
		void OnSuspending( ::Platform::Object^ sender, ::Windows::ApplicationModel::SuspendingEventArgs^ e );

#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
		void OnRootFrameFirstNavigated( ::Platform::Object^ sender, ::Windows::UI::Xaml::Navigation::NavigationEventArgs^ e );
#endif

	private:
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
		::Windows::Foundation::EventRegistrationToken navigatedEventToken_;
		::Windows::UI::Xaml::Media::Animation::TransitionCollection^ transitions_;
#endif
	};

} } } }