#include "pch.h"
#include "./mnfx/window.h"
#include "comment_window.h"

using namespace mnfx;

comment_window::comment_window( const std::shared_ptr<virtual_comment_server> cmtsvr )
	: cmtsvr_( cmtsvr )
{
	set_text( L"コメント入力ウィンドウ" );
	set_left( 48 );
	set_height( 81 );
	set_width( 526 );
	set_style( style() & ~( WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME ) );
}

LRESULT comment_window::window_procedure( window_message message, WPARAM wparam, LPARAM lparam, bool& handled ) noexcept
{
	if( message == window_message::command )
	{
		const HWND& hwnd = reinterpret_cast<HWND>( lparam );
		if( post_button_.is_self( hwnd ) )
		{
			auto comment = std::make_shared<comment_sample>(
				comment_box_.text(),
				true,
				false,
				nico::renderer::comment_vertical_position_type::center,
				nico::renderer::comment_size_type::medium,
				0xffffff );
			cmtsvr_->post( comment );
			comment_box_.set_text( L"" );
		}
		else if( autotest_button_.is_self( hwnd ) )
		{
			cmtsvr_->autotest();
		}
		else if( reset_button_.is_self( hwnd ) )
		{
			cmtsvr_->reset();
		}
		else if( regular_radio_.is_self( hwnd ) )
		{
			top_radio_.set_checked( false );
			both_radio_.set_checked( false );
			bottom_radio_.set_checked( false );
			cmtsvr_->change_mode( nico::renderer::comment_mode_type::default );
		}
		else if( top_radio_.is_self( hwnd ) )
		{
			regular_radio_.set_checked( false );
			both_radio_.set_checked( false );
			bottom_radio_.set_checked( false );
			cmtsvr_->change_mode( nico::renderer::comment_mode_type::top );
		}
		else if( both_radio_.is_self( hwnd ) )
		{
			regular_radio_.set_checked( false );
			top_radio_.set_checked( false );
			bottom_radio_.set_checked( false );
			cmtsvr_->change_mode( nico::renderer::comment_mode_type::both );
		}
		else if( bottom_radio_.is_self( hwnd ) )
		{
			regular_radio_.set_checked( false );
			top_radio_.set_checked( false );
			both_radio_.set_checked( false );
			cmtsvr_->change_mode( nico::renderer::comment_mode_type::bottom );
		}
	}
	return window::window_procedure( message, wparam, lparam, handled );
}

HRESULT comment_window::initialize( HINSTANCE hinstance, const window& parent ) noexcept
{
	HRESULT hr = S_OK;

	hr = mnfx::window::initialize( hinstance, parent, L"nico renderer demo app - comment window" );

	set_top( 48 + this->parent()->bottom() );

	return hr;
}

HRESULT comment_window::on_create() noexcept
{
	command_box_.set_top( default_window_padding );
	command_box_.set_left( default_window_padding );
	command_box_.set_width( 120 );
	add_editbox( command_box_ );

	comment_box_.set_top( default_window_padding );
	comment_box_.set_left( related_control_spacing + command_box_.right() );
	comment_box_.set_width( width() - comment_box_.left() - ( related_control_spacing + default_width + default_window_padding ) );
	add_editbox( comment_box_ );

	post_button_.set_text( L"投稿" );
	post_button_.set_top( default_window_padding );
	post_button_.set_left( width() - default_window_padding - post_button_.width() );
	add_button( post_button_ );

	regular_radio_.set_text( L"通常" );
	regular_radio_.set_top( unrelated_control_spacing + command_box_.bottom() );
	regular_radio_.set_left( default_window_padding );
	regular_radio_.set_width( 60 );
	regular_radio_.set_checked( true );
	add_button( regular_radio_ );

	top_radio_.set_text( L"上" );
	top_radio_.set_top( unrelated_control_spacing + command_box_.bottom() );
	top_radio_.set_left( regular_radio_.right() );
	top_radio_.set_width( 60 );
	add_button( top_radio_ );

	both_radio_.set_text( L"両方" );
	both_radio_.set_top( unrelated_control_spacing + command_box_.bottom() );
	both_radio_.set_left( top_radio_.right() );
	both_radio_.set_width( 60 );
	add_button( both_radio_ );

	bottom_radio_.set_text( L"下" );
	bottom_radio_.set_top( unrelated_control_spacing + command_box_.bottom() );
	bottom_radio_.set_left( both_radio_.right() );
	bottom_radio_.set_width( 60 );
	add_button( bottom_radio_ );

	autotest_button_.set_text( L"オートテスト" );
	autotest_button_.set_top( related_control_spacing + post_button_.bottom() );
	autotest_button_.set_left( width() - default_window_padding - autotest_button_.width() );
	add_button( autotest_button_ );

	reset_button_.set_text( L"リセット" );
	reset_button_.set_top( autotest_button_.top() );
	reset_button_.set_left( autotest_button_.left() - related_control_spacing - autotest_button_.width() );
	add_button( reset_button_ );

	return S_OK;
}