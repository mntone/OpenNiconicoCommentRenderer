#include "pch.h"
#include "MainPage.xaml.h"

using namespace Concurrency;
using namespace Platform;
using namespace std;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;
using namespace Mntone::Nico::Renderer::Demo::Data;
using namespace Mntone::Nico::Renderer::Demo::ViewModels;
using namespace Mntone::Nico::Renderer::Demo::Views;

String^ visualStateGroupScreenMode = "ScreenMode";
String^ visualStateDefaultScreen = "DefaultScreen";
String^ visualStateFullScreen = "FullScreen";

MainPage::MainPage()
{
	InitializeComponent();

	auto displayInformation = DisplayInformation::GetForCurrentView();
	displayInformation->OrientationChanged += ref new TypedEventHandler<DisplayInformation^, Object^>( this, &MainPage::OnOrientationChanged );
	OnOrientationChanged( displayInformation, nullptr );

	auto wr = WeakReference( this );
	PageViewModel = ref new ::PageViewModel();
	PageViewModel->CommentServer->CommentReceived += ref new Core::CommentReceivedEventHandler( [wr]( SimpleComment^ comment )
	{
		auto that = wr.Resolve<MainPage>();
		that->commentLayer->AddComment( comment );
	} );
	PageViewModel->CommentModeChanged += ref new DependencyPropertyChangedEventHandler( [wr]( Object^ sender, DependencyPropertyChangedEventArgs^ e )
	{
		auto newValue = dynamic_cast<IBox<int32>^>( e->NewValue );
		if( newValue )
		{
			auto that = wr.Resolve<MainPage>();
			that->commentLayer->CommentMode = static_cast<CommentModeType>( newValue->Value + 1 );
		}
	} );
	PageViewModel->CommentScaleChanged += ref new DependencyPropertyChangedEventHandler( [wr]( Object^ sender, DependencyPropertyChangedEventArgs^ e )
	{
		auto newValue = dynamic_cast<IBox<float32>^>( e->NewValue );
		if( newValue )
		{
			auto that = wr.Resolve<MainPage>();
			that->commentLayer->CommentScale = newValue->Value;
		}
	} );
}

void MainPage::OnPageLoaded( Object^ sender, RoutedEventArgs^ e )
{
	OnOuterSizeChanged( sender, nullptr );
}

void MainPage::OnNavigatedTo( NavigationEventArgs^ e )
{
	commentLayer->StartRendering();
}

void MainPage::OnNavigatedFrom( NavigationEventArgs^ e )
{
	commentLayer->EndRendering();
}

void MainPage::OnOuterLoaded( Object^ sender, RoutedEventArgs^ e )
{
	initialized_ = true;
}

void MainPage::OnOuterSizeChanged( Object^ sender, SizeChangedEventArgs^ e )
{
	if( !initialized_ )
	{
		return;
	}

	// 7:4 (動画: 16:9)
	const auto actualHeight = static_cast<float32>( outer->ActualHeight );
	const auto actualWidth = static_cast<float32>( outer->ActualWidth );

	bool isComment = commentLayerSwitch->IsOn;
	bool isWide = 9 /*media->NaturalVideoHeight*/ != 0 ? static_cast<float32>( /*media->NaturalVideoWidth*/ 16 ) / /*media->NaturalVideoHeight*/ 9 > 1.4f : true;

	if( isComment )
	{
		commentLayer->IsEnabled = true;
		commentLayer->Visibility = ::Visibility::Visible;

		if( fullScreen_ )
		{
			const auto innerWidth = 7.f * actualHeight / 4.f;
			if( innerWidth > actualWidth )
			{
				const auto innerHeight = 4.f * actualWidth / 7.f;
				const auto mediaAreaWidth = isWide ? ( 20.f / 21.f ) * actualWidth : ( 16.f / 21.f ) * actualWidth;
				media->Height = isWide ? 9.f * mediaAreaWidth / 16.f : 3.f * mediaAreaWidth / 4.f;
				media->Width = mediaAreaWidth;
				commentLayer->Height = innerHeight;
				commentLayer->Width = actualWidth;
				inner->Height = innerHeight;
				inner->Width = actualWidth;
			}
			else
			{
				const auto mediaAreaHeight = isWide ? ( 45.f / 48.f ) * actualHeight : actualHeight;
				media->Height = mediaAreaHeight;
				media->Width = isWide ? 16.f * mediaAreaHeight / 9.f : 4.f * mediaAreaHeight / 3.f;
				commentLayer->Height = actualHeight;
				commentLayer->Width = innerWidth;
				inner->Height = actualHeight;
				inner->Width = innerWidth;
			}
		}
		else
		{
			const auto innerHeight = 4.f * actualWidth / 7.f;
			const auto mediaAreaWidth = isWide ? ( 20.f / 21.f ) * actualWidth : ( 16.f / 21.f ) * actualWidth;
			media->Height = isWide ? 9.f * mediaAreaWidth / 16.f : 3.f * mediaAreaWidth / 4.f;
			media->Width = mediaAreaWidth;
			commentLayer->Height = innerHeight;
			commentLayer->Width = actualWidth;
			inner->Height = innerHeight;
			inner->Width = actualWidth;
		}
		if( backComment_ )
		{
			media->Height *= 0.8f;
			media->Width *= 0.8f;
			Canvas::SetZIndex( commentLayer, 0 );
		}
		else
		{
			Canvas::SetZIndex( commentLayer, 2 );
		}
		Canvas::SetTop( media, 0.5f * ( inner->Height - media->Height ) );
		Canvas::SetLeft( media, 0.5f * ( inner->Width - media->Width ) );
	}
	else
	{
		commentLayer->IsEnabled = false;
		commentLayer->Visibility = ::Visibility::Collapsed;

		if( fullScreen_ )
		{
			const auto innerWidth = isWide ? 16.f * actualHeight / 9.f : 4.f * actualHeight / 3.f;
			if( innerWidth > actualWidth )
			{
				const auto innerHeight = isWide ? 9.f * actualWidth / 16.f : 3.f * actualWidth / 4.f;
				media->Height = innerHeight;
				media->Width = actualWidth;
				inner->Height = innerHeight;
				inner->Width = actualWidth;
			}
			else
			{
				media->Height = actualHeight;
				media->Width = innerWidth;
				inner->Height = actualHeight;
				inner->Width = innerWidth;
			}
		}
		else
		{
			const auto innerHeight = isWide ? 9.f * actualWidth / 16.f : 3.f * actualHeight / 4.f;
			media->Height = innerHeight;
			media->Width = actualWidth;
			inner->Height = innerHeight;
			inner->Width = actualWidth;
		}
		Canvas::SetTop( media, 0.f );
		Canvas::SetLeft( media, 0.f );
	}
}

void MainPage::OnSwitchToggled( Object^ sender, RoutedEventArgs^ e )
{
	OnOuterSizeChanged( sender, nullptr );
}

void MainPage::OnResetButtonClick( Object^ sender, RoutedEventArgs^ e )
{
	commentLayer->ResetComment();
}

void MainPage::OnOrientationChanged( DisplayInformation^ sender, Object^ args )
{
	lock_guard<mutex> lock( fullScreenMutex_ );
	if( sender->CurrentOrientation == DisplayOrientations::Portrait )
	{
		StatusBar::GetForCurrentView()->ShowAsync();
		VisualStateManager::GoToState( this, visualStateDefaultScreen, false );
		fullScreen_ = false;
	}
	else
	{
		StatusBar::GetForCurrentView()->HideAsync();
		VisualStateManager::GoToState( this, visualStateFullScreen, false );
		fullScreen_ = true;
	}
}

void MainPage::OnComboBoxSelectionChanged( Object^ sender, SelectionChangedEventArgs^ e )
{
	backComment_ = commentModeComboBox->SelectedIndex == 4;
	OnOuterSizeChanged( sender, nullptr );
}

IMPL_DP_GETSET( MainPage, ::PageViewModel, PageViewModel, nullptr );