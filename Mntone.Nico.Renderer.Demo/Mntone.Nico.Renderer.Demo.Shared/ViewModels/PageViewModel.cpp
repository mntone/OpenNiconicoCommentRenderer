#include "pch.h"
#include "PageViewModel.h"

using namespace Mntone::Xamcc::Commands;
using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Mntone::Nico::Renderer;
using namespace Mntone::Nico::Renderer::Demo::Data;
using namespace Mntone::Nico::Renderer::Demo::ViewModels;

PageViewModel::PageViewModel()
{
	CommentServer_ = ref new Core::VirtualCommentServerService();

	SendCommand = ref new RelayCommand( [this]( Object^ )
	{
		CommentServer_->AddComment( ref new SimpleComment(
			Value,
			true,
			false,
			CommentVerticalPositionType::Center,
			CommentSizeType::Medium,
			0xffffff ) );
		Value = "";
	} );
	AutoTestCommand = ref new RelayCommand( [this]( Object^ )
	{
		CommentServer_->AddComment( ref new SimpleComment(
			"1\n2\n3\n4\n5\n6",
			true,
			false,
			CommentVerticalPositionType::Center,
			CommentSizeType::Medium,
			0xffffff ) );
		CommentServer_->AddComment( ref new SimpleComment(
			"テスト",
			true,
			false,
			CommentVerticalPositionType::Center,
			CommentSizeType::Medium,
			0xffffff ) );
		CommentServer_->AddComment( ref new SimpleComment(
			"わこつ",
			true,
			false,
			CommentVerticalPositionType::Center,
			CommentSizeType::Medium,
			0xffffff ) );
		CommentServer_->AddComment( ref new SimpleComment(
			L"⒮←丸ゴシック (Gulim) 化文字",
			false,
			false,
			CommentVerticalPositionType::Center,
			CommentSizeType::Large,
			0xffffff ) );
		CommentServer_->AddComment( ref new SimpleComment(
			L"⑴←明朝 (SimSun) 化文字",
			false,
			false,
			CommentVerticalPositionType::Center,
			CommentSizeType::Large,
			0xffffff ) );
		CommentServer_->AddComment( ref new SimpleComment(
			L"ˍ←明朝 (PMingLiU) 化文字",
			false,
			false,
			CommentVerticalPositionType::Center,
			CommentSizeType::Large,
			0xffffff ) );
		CommentServer_->AddComment( ref new SimpleComment(
			"Visual Studio is cool!",
			false,
			false,
			CommentVerticalPositionType::Bottom,
			CommentSizeType::Small,
			0xffffff ) );
		CommentServer_->AddComment( ref new SimpleComment(
			"ながあああああああああああああいおつきあい。京都銀行",
			false,
			false,
			CommentVerticalPositionType::Center,
			CommentSizeType::Medium,
			0xffffff ) );
		CommentServer_->AddComment( ref new SimpleComment(
			"Super Mario BROS. プレイしようぜ!",
			false,
			false,
			CommentVerticalPositionType::Center,
			CommentSizeType::Medium,
			0 ) );
		CommentServer_->AddComment( ref new SimpleComment(
			"てってってー",
			false,
			false,
			CommentVerticalPositionType::Center,
			CommentSizeType::Large,
			0x00cc66 ) );
	} );
}

void PageViewModel::OnCommentModeChanged( DependencyObject^ d, DependencyPropertyChangedEventArgs^ e )
{
	auto that = dynamic_cast<PageViewModel^>( d );
	if( that )
	{
		that->CommentModeChanged( that, e );
	}
}

IMPL_DP_GETSET( PageViewModel, String, Value, PropertyMetadata::Create( "" ) )

CommentModeType PageViewModel::CommentMode::get()
{
	auto intValue = dynamic_cast<IBox<int32>^>( GetValue( CommentModeProperty_ ) );
	return intValue ? static_cast<CommentModeType>( intValue->Value + 1 ) : static_cast<CommentModeType>( 1 );
}

void PageViewModel::CommentMode::set( CommentModeType value )
{
	SetValue( CommentModeProperty_, static_cast<int32>( value ) - 1 );
}

DependencyProperty^ PageViewModel::CommentModeProperty_ = DependencyProperty::Register(
	"CommentMode",
	int::typeid,
	PageViewModel::typeid,
	PropertyMetadata::Create( 0, ref new PropertyChangedCallback( &PageViewModel::OnCommentModeChanged ) ) );

IMPL_DP_GETSET( PageViewModel, RelayCommand, SendCommand, nullptr )
IMPL_DP_GETSET( PageViewModel, RelayCommand, AutoTestCommand, nullptr )