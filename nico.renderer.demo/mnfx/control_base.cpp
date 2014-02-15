/*
	about mntoneÅfs framework for Windows

	The MIT License (MIT)

	Copyright (c) 2014 mntone

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include "pch.h"
#include "control_base.h"
#include "window.h"

using namespace mnfx;

NONCLIENTMETRICSW control_base::non_client_metrics_;
HFONT control_base::gui_font_;

control_base::control_base()
	: initialized_( false )
	, enable_( true )
	, hwnd_( nullptr )
	, text_( L"" )
	, style_( WS_CHILD | WS_VISIBLE | WS_TABSTOP )
	, top_( default_top )
	, left_( default_left )
	, height_( default_height )
	, width_( default_width )
{
	if( !gui_font_ )
	{
		non_client_metrics_ = { sizeof( NONCLIENTMETRICSW ) };
		SystemParametersInfoW( SPI_GETNONCLIENTMETRICS, sizeof( NONCLIENTMETRICSW ), &non_client_metrics_, 0 );
		gui_font_ = CreateFontIndirectW( &non_client_metrics_.lfMessageFont );
	}
}

control_base::~control_base()
{
	if( hwnd_ )
	{
		DestroyWindow( hwnd_ );
		hwnd_ = nullptr;
	}
}

HRESULT control_base::initialize( const window& parent, LPCWSTR class_name, int32_t tabstop ) noexcept
{
	parent_ = &parent;

	hwnd_ = CreateWindowExW(
		0,
		class_name,
		text_.c_str(),
		style_,
		static_cast<int>( std::floor( parent_->scale_factor_.scale_x( left_ ) ) ),
		static_cast<int>( std::floor( parent_->scale_factor_.scale_y( top_ ) ) ),
		static_cast<int>( std::ceil( parent_->scale_factor_.scale_x( width_ ) ) ),
		static_cast<int>( std::ceil( parent_->scale_factor_.scale_y( height_ ) ) ),
		parent_->hwnd(),
		reinterpret_cast<HMENU>( tabstop ),
		parent_->hinstance(),
		nullptr );
	if( !hwnd_ )
	{
		return E_FAIL;
	}

	SendMessageW( hwnd_, WM_SETFONT, reinterpret_cast<WPARAM>( gui_font_ ), TRUE );

	initialized_ = true;
	return S_OK;
}

HRESULT control_base::set_text() noexcept
{
	return SetWindowTextW( hwnd_, text_.c_str() ) != 0 ? S_OK : E_FAIL;
}

HRESULT control_base::set_position_and_size() noexcept
{
	return SetWindowPos(
		hwnd_,
		nullptr,
		static_cast<int>( std::floor( parent_->scale_factor_.scale_x( left_ ) ) ),
		static_cast<int>( std::floor( parent_->scale_factor_.scale_y( top_ ) ) ),
		static_cast<int>( std::ceil( parent_->scale_factor_.scale_x( width_ ) ) ),
		static_cast<int>( std::ceil( parent_->scale_factor_.scale_y( height_ ) ) ),
		SWP_NOZORDER ) != 0 ? S_OK : E_FAIL;
}

HRESULT control_base::set_position() noexcept
{
	return SetWindowPos(
		hwnd_,
		nullptr,
		static_cast<int>( std::floor( parent_->scale_factor_.scale_x( left_ ) ) ),
		static_cast<int>( std::floor( parent_->scale_factor_.scale_y( top_ ) ) ),
		static_cast<int>( std::ceil( parent_->scale_factor_.scale_x( width_ ) ) ),
		static_cast<int>( std::ceil( parent_->scale_factor_.scale_y( height_ ) ) ),
		SWP_NOZORDER | SWP_NOSIZE ) != 0 ? S_OK : E_FAIL;
}

HRESULT control_base::set_size() noexcept
{
	return SetWindowPos(
		hwnd_,
		nullptr,
		static_cast<int>( std::floor( parent_->scale_factor_.scale_x( left_ ) ) ),
		static_cast<int>( std::floor( parent_->scale_factor_.scale_y( top_ ) ) ),
		static_cast<int>( std::ceil( parent_->scale_factor_.scale_x( width_ ) ) ),
		static_cast<int>( std::ceil( parent_->scale_factor_.scale_y( height_ ) ) ),
		SWP_NOZORDER | SWP_NOMOVE ) != 0 ? S_OK : E_FAIL;
}

HRESULT control_base::set_style() noexcept
{
	return SetWindowLongPtrW( hwnd_, GWL_STYLE, style_ ) != 0 ? S_OK : E_FAIL;
}

void control_base::set_enable() noexcept
{
	EnableWindow( hwnd_, enable_ ? TRUE : FALSE );
}

void control_base::set_text( std::wstring value ) noexcept
{
	text_ = std::move( value );
	if( initialized_ )
	{
		set_text();
	}
}

void control_base::set_top( dialog_unit value ) noexcept
{
	top_ = value;
	if( initialized_ )
	{
		set_position();
	}
}

void control_base::set_left( dialog_unit value ) noexcept
{
	left_ = value;
	if( initialized_ )
	{
		set_position();
	}
}

void control_base::set_height( dialog_unit value ) noexcept
{
	height_ = value;
	if( initialized_ )
	{
		set_size();
	}
}

void control_base::set_width( dialog_unit value ) noexcept
{
	width_ = value;
	if( initialized_ )
	{
		set_size();
	}
}

void control_base::set_style( DWORD value ) noexcept
{
	style_ = std::move( value );
	if( initialized_ )
	{
		set_style();
	}
}

void control_base::set_enable( bool value ) noexcept
{
	enable_ = std::move( value );
	if( initialized_ )
	{
		set_enable();
	}
}


void radio_button::set_checked() noexcept
{
	SendMessageW( hwnd(), BM_SETCHECK, checked_ ? TRUE : FALSE, 0 );
}

void radio_button::set_checked( bool value ) noexcept
{
	checked_ = std::move( value );
	if( initialized() )
	{
		set_checked();
	}
}


const std::wstring& edit_box::text() const noexcept
{
	if( initialized() )
	{
		const auto& length = static_cast<std::wstring::size_type>( SendMessageW( hwnd(), WM_GETTEXTLENGTH, 0, 0 ) );
		text_.resize( length + 1 );
		SendMessageW( hwnd(), WM_GETTEXT, length + 1, reinterpret_cast<LPARAM>( &text_[0] ) );
	}
	return text_;
}