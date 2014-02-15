/*
	about mntonefs framework for Windows

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
#include "window.h"

using namespace mnfx;

window::window()
	: parent_( nullptr )
	, initialized_( false )
	, hinstance_( nullptr )
	, hwnd_( nullptr )
	, text_( L"" )
	, class_name_( L"" )
	, style_( WS_OVERLAPPEDWINDOW | WS_VISIBLE )
	, top_( 48 )
	, left_( 48 )
	, height_( 480 )
	, width_( 640 )
	, tabstop_( 1 )
{ }

HRESULT window::initialize( HINSTANCE hinstance, std::wstring class_name ) noexcept
{
	return initialize( std::move( hinstance ), nullptr, std::move( class_name ) );
}

HRESULT window::initialize( HINSTANCE hinstance, const window& parent_window, std::wstring class_name ) noexcept
{
	return initialize( std::move( hinstance ), &parent_window, std::move( class_name ) );
}

HRESULT window::initialize( HINSTANCE hinstance, const window* const parent_window, std::wstring class_name ) noexcept
{
	HRESULT hr = S_OK;

	hinstance_ = std::move( hinstance );
	parent_ = parent_window;
	class_name_ = std::move( class_name );

	hr = register_window_class();
	if( FAILED( hr ) )
	{
		return hr;
	}

	CreateWindowExW(
		WS_EX_CONTROLPARENT,
		class_name_.c_str(),
		text_.c_str(),
		style_,
		0, 0, 0, 0,
		parent_ ? parent_->hwnd() : nullptr,
		nullptr,
		hinstance_,
		this );
	if( !hwnd_ )
	{
		return E_FAIL;
	}

	set_position_and_size();
	UpdateWindow( hwnd_ );

	initialized_ = true;

	return S_OK;
}

void window::run() noexcept
{
	MSG msg;
	while( GetMessageW( &msg, nullptr, 0, 0 ) )
	{
		if( !IsDialogMessageW( hwnd(), &msg ) )
		{
			TranslateMessage( &msg );
			DispatchMessageW( &msg );
		}
	}
}

HRESULT window::add_button( button& control ) noexcept
{
	return control.initialize( *this, tabstop_++ );
}

HRESULT window::add_editbox( edit_box& control ) noexcept
{
	return control.initialize( *this, tabstop_++ );
}

LRESULT window::window_procedure( window_message message, WPARAM wparam, LPARAM lparam, bool& handled ) noexcept
{
	switch( message )
	{
	case window_message::size:
		prepare_resize( lparam );
		handled = true;
		break;

	case window_message::create:
		on_create();
		handled = true;
		break;

	case window_message::destroy:
		PostQuitMessage( 0 );
		handled = true;
		break;

	case window_message::dpi_changed:
		prepare_dpi_changed( wparam, lparam );
		handled = true;
		break;
	}

	return 0;
}

HRESULT window::register_window_class() noexcept
{
	WNDCLASSEXW wcex = { sizeof( WNDCLASSEXW ) };
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = &window_procedure_lancher;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof( window* );
	wcex.hInstance = hinstance_;
	wcex.hIcon = LoadIconW( nullptr, IDI_APPLICATION );
	wcex.hCursor = LoadCursorW( nullptr, IDC_ARROW );
	wcex.hbrBackground = reinterpret_cast<HBRUSH>( COLOR_WINDOW );
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = class_name_.c_str();
	wcex.hIconSm = LoadIconW( nullptr, IDI_APPLICATION );
	return RegisterClassExW( &wcex ) ? S_OK : E_FAIL;
}

HRESULT window::set_text() noexcept
{
	return SetWindowTextW( hwnd_, text_.c_str() ) != 0 ? S_OK : E_FAIL;
}

RECT window::get_window_size_from_client_area() noexcept
{
	RECT rect =
	{
		left_,
		top_,
		left_ + static_cast<physical_unit>( std::ceil( scale_factor_.scale_x( width_ ) ) ),
		top_ + static_cast<physical_unit>( std::ceil( scale_factor_.scale_y( height_ ) ) )
	};
	AdjustWindowRectEx( &rect, style_, FALSE, 0 );
	return rect;
}

HRESULT window::set_position_and_size() noexcept
{
	const RECT& rect = get_window_size_from_client_area();
	return SetWindowPos( hwnd_, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER ) != 0 ? S_OK : E_FAIL;
}

HRESULT window::set_position() noexcept
{
	const RECT& rect = get_window_size_from_client_area();
	return SetWindowPos( hwnd_, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOSIZE ) != 0 ? S_OK : E_FAIL;
}

HRESULT window::set_size() noexcept
{
	const RECT& rect = get_window_size_from_client_area();
	return SetWindowPos( hwnd_, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOMOVE ) != 0 ? S_OK : E_FAIL;
}

HRESULT window::set_style() noexcept
{
	return SetWindowLongPtrW( hwnd_, GWL_STYLE, style_ ) != 0 ? S_OK : E_FAIL;
}

HRESULT window::prepare_resize( LPARAM lparam ) noexcept
{
	if( initialized_ )
	{
		return on_resize(
			scale_factor_.scale_inverse_y( HIWORD( lparam ) ),
			scale_factor_.scale_inverse_x( LOWORD( lparam ) ) );
	}
	return S_OK;
}

HRESULT window::prepare_dpi_changed( WPARAM wparam, LPARAM lparam )
{
	const uint16_t& ydpi = HIWORD( wparam );
	const uint16_t& xdpi = LOWORD( wparam );
	const auto& scale = scale_factor_.set_dpi( ydpi, xdpi );
	height_ = scale.first * static_cast<dpi_scale_factor::dpi_unit>( height_ );
	width_ = scale.second * static_cast<dpi_scale_factor::dpi_unit>( width_ );

	const RECT& suggest = *reinterpret_cast<LPRECT>( lparam );
	if( initialized_ )
	{
		set_size();
	}

	return on_dpi_changed( suggest );
}

LRESULT CALLBACK window::window_procedure_lancher( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{
	if( message == WM_CREATE )
	{
		LPCREATESTRUCT create_struct = reinterpret_cast<LPCREATESTRUCT>( lparam );
		window* wnd = static_cast<window*>( create_struct->lpCreateParams );
		if( wnd == nullptr )
		{
			exit( 1 );
		}

		wnd->hwnd_ = hwnd;
		wnd->scale_factor_.initialize( MonitorFromWindow( wnd->hwnd_, MONITOR_DEFAULTTONEAREST ) );
		SetWindowLongPtrW( hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( wnd ) );

		bool handled = false;
		const LRESULT& lresult = wnd->window_procedure( static_cast<window_message>( message ), wparam, lparam, handled );
		if( !handled )
		{
			return DefWindowProcW( hwnd, message, wparam, lparam );
		}
		return lresult;
	}

	window* wnd = reinterpret_cast<window*>( static_cast<LONG_PTR>( GetWindowLongPtrW( hwnd, GWLP_USERDATA ) ) );
	if( wnd == nullptr )
	{
		return DefWindowProcW( hwnd, message, wparam, lparam );
	}

	bool handled = false;
	const LRESULT& lresult = wnd->window_procedure( static_cast<window_message>( message ), wparam, lparam, handled );
	if( !handled )
	{
		return DefWindowProcW( hwnd, message, wparam, lparam );
	}
	return lresult;
}

void window::set_text( std::wstring value ) noexcept
{
	text_ = std::move( value );
	if( initialized_ )
	{
		set_text();
	}
}

void window::set_top( physical_unit value ) noexcept
{
	top_ = value;
	if( initialized_ )
	{
		set_position();
	}
}

void window::set_left( physical_unit value ) noexcept
{
	left_ = value;
	if( initialized_ )
	{
		set_position();
	}
}

void window::set_height( dialog_unit value ) noexcept
{
	height_ = value;
	if( initialized_ )
	{
		set_size();
	}
}

void window::set_width( dialog_unit value ) noexcept
{
	width_ = value;
	if( initialized_ )
	{
		set_size();
	}
}

void window::set_style( DWORD value ) noexcept
{
	style_ = std::move( value );
	if( initialized_ )
	{
		set_style();
	}
}