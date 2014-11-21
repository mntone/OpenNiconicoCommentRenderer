#pragma once
#include "./mnfx/window.h"
#include "virtual_comment_server.h"

class comment_window final
	: public mnfx::window
{
public:
	comment_window( const std::shared_ptr<virtual_comment_server> cmtsvr );
	virtual ~comment_window() { }

public:
	virtual HRESULT initialize( HINSTANCE hinstance, const window& parent ) noexcept;

protected:
	virtual LRESULT window_procedure( mnfx::window_message message, WPARAM wparam, LPARAM lparam, bool& handled ) noexcept;
	HRESULT on_create() noexcept;

private:
	std::shared_ptr<virtual_comment_server> cmtsvr_;

	mnfx::edit_box command_box_, comment_box_, scale_box_;
	mnfx::radio_button regular_radio_, top_radio_, both_radio_, bottom_radio_;
	mnfx::button post_button_, autotest_button_, reset_button_, set_scale_button_;
};