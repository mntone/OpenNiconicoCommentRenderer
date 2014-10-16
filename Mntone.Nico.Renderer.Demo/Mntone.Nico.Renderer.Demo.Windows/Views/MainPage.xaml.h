#pragma once
#include "Views\MainPage.g.h"
#include "Data\SimpleComment.h"
#include "ViewModels\PageViewModel.h"

namespace Mntone { namespace Nico { namespace Renderer { namespace Demo { namespace Views {

	public ref class MainPage sealed
	{
	public:
		MainPage();

	protected:
		virtual void OnNavigatedTo( ::Windows::UI::Xaml::Navigation::NavigationEventArgs^ e ) override;
		virtual void OnNavigatedFrom( ::Windows::UI::Xaml::Navigation::NavigationEventArgs^ e ) override;

	private:
		void OnPageLoaded( ::Platform::Object^ sender, ::Windows::UI::Xaml::RoutedEventArgs^ e );
		void OnPageKeyUp( ::Platform::Object^ sender, ::Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e );

		void OnOuterLoaded( ::Platform::Object^ sender, ::Windows::UI::Xaml::RoutedEventArgs^ e );
		void OnOuterSizeChanged( ::Platform::Object^ sender, ::Windows::UI::Xaml::SizeChangedEventArgs^ e );
		void OnMediaOpened( ::Platform::Object^ sender, ::Windows::UI::Xaml::RoutedEventArgs^ e );

		void OnPlayButtonClick( ::Platform::Object^ sender, ::Windows::UI::Xaml::RoutedEventArgs^ e );
		void OnFullScreenButtonClick( ::Platform::Object^ sender, ::Windows::UI::Xaml::RoutedEventArgs^ e );
		void OnSwitchToggled( ::Platform::Object^ sender, ::Windows::UI::Xaml::RoutedEventArgs^ e );

		void OnComboBoxSelectionChanged( ::Platform::Object^ sender, ::Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e );

	public:
		property ViewModels::PageViewModel^ PageViewModel
		{
			ViewModels::PageViewModel^ get();
			void set( ViewModels::PageViewModel^ value );
		}

		static property ::Windows::UI::Xaml::DependencyProperty^ PageViewModelProperty
		{
			::Windows::UI::Xaml::DependencyProperty^ get() { return PageViewModelProperty_; }
		}

	private:
		std::mutex fullScreenMutex_;
		bool initialized_, fullScreen_, backComment_;

		static ::Windows::UI::Xaml::DependencyProperty^ PageViewModelProperty_;
	};
} } } } }