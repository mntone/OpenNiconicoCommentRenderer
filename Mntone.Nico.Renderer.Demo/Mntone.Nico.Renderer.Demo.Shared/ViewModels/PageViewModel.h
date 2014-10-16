#pragma once
#include "Core\VirtualCommentServerService.h"

namespace Mntone { namespace Nico { namespace Renderer { namespace Demo { namespace ViewModels {

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PageViewModel sealed
		: ::Windows::UI::Xaml::DependencyObject
	{
	public:
		PageViewModel();

	private:
		static void OnCommentModeChanged( ::Windows::UI::Xaml::DependencyObject^ d, ::Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ e );
		
	public:
		event Windows::UI::Xaml::DependencyPropertyChangedEventHandler^ CommentModeChanged;

	internal:
		property Core::VirtualCommentServerService^ CommentServer
		{
			Core::VirtualCommentServerService^ get() { return CommentServer_; }
		}

		property ::Platform::String^ Value
		{
			::Platform::String^ get();
			void set( ::Platform::String^ value );
		}
		property CommentModeType CommentMode
		{
			CommentModeType get();
			void set( CommentModeType value );
		}
		property Xamcc::Commands::RelayCommand^ SendCommand
		{
			Xamcc::Commands::RelayCommand^ get();
			void set( Xamcc::Commands::RelayCommand^ value );
		}
		property Xamcc::Commands::RelayCommand^ AutoTestCommand
		{
			Xamcc::Commands::RelayCommand^ get();
			void set( Xamcc::Commands::RelayCommand^ value );
		}

		static property ::Windows::UI::Xaml::DependencyProperty^ ValueProperty
		{
			::Windows::UI::Xaml::DependencyProperty^ get() { return ValueProperty_; }
		}
		static property ::Windows::UI::Xaml::DependencyProperty^ SendCommandProperty
		{
			::Windows::UI::Xaml::DependencyProperty^ get() { return SendCommandProperty_; }
		}
		static property ::Windows::UI::Xaml::DependencyProperty^ AutoTestCommandProperty
		{
			::Windows::UI::Xaml::DependencyProperty^ get() { return AutoTestCommandProperty_; }
		}
		static property ::Windows::UI::Xaml::DependencyProperty^ CommentModeProperty
		{
			::Windows::UI::Xaml::DependencyProperty^ get() { return CommentModeProperty_; }
		}

	private:
		Core::VirtualCommentServerService^ CommentServer_;

		static ::Windows::UI::Xaml::DependencyProperty
			^ ValueProperty_,
			^ SendCommandProperty_,
			^ AutoTestCommandProperty_,
			^ CommentModeProperty_;
	};

} } } } }