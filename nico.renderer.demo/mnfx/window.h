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

#pragma once
#include <vector>
#include "control_base.h"
#include "dpi_scale_factor.h"
#include "window_message.h"

namespace mnfx {

	using physical_unit = int32_t;
	using dialog_unit = float;

	const dialog_unit default_window_padding = 11;
	const dialog_unit related_control_spacing = 7;
	const dialog_unit unrelated_control_spacing = 11;

	class window
	{
		friend class directx_window;
		friend class control_base;

	public:
		window();

	public:
		virtual HRESULT initialize( HINSTANCE hinstance, std::wstring class_name ) noexcept;
		virtual HRESULT initialize( HINSTANCE hinstance, const window& parent_window, std::wstring class_name ) noexcept;
		virtual void run() noexcept;

		HRESULT add_button( button& control ) noexcept;
		HRESULT add_editbox( edit_box& control ) noexcept;

	protected:
		virtual LRESULT window_procedure( window_message message, WPARAM wparam, LPARAM lparam, bool& handled ) noexcept;
		virtual HRESULT register_window_class() noexcept;

		HRESULT set_text() noexcept;
		HRESULT set_position_and_size() noexcept;
		HRESULT set_position() noexcept;
		HRESULT set_size() noexcept;
		HRESULT set_style() noexcept;

		virtual HRESULT on_create() noexcept
		{
			return S_OK;
		}
		virtual HRESULT on_resize( dialog_unit height, dialog_unit width ) noexcept
		{
			height_ = height;
			width_ = width;
			return S_OK;
		}
		virtual HRESULT on_dpi_changed( const RECT /*suggest*/ ) const noexcept
		{
			return S_OK;
		}

	private:
		HRESULT initialize( HINSTANCE hinstance, const window* const parent, std::wstring class_name ) noexcept;

		RECT get_window_size_from_client_area() noexcept;

		HRESULT prepare_resize( LPARAM lparam ) noexcept;
		HRESULT prepare_dpi_changed( WPARAM wparam, LPARAM lparam );

		static LRESULT CALLBACK window_procedure_lancher( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );

	public:
		const window* const parent() const noexcept { return parent_; }
		const std::wstring& class_name() const noexcept { return class_name_; }
		const HINSTANCE& hinstance() const noexcept { return hinstance_; }
		const HWND& hwnd() const noexcept { return hwnd_; }
			
		const std::wstring& text() const noexcept { return text_; }
		virtual void set_text( std::wstring value ) noexcept;

		const physical_unit top() const noexcept { return top_; }
		virtual void set_top( physical_unit value ) noexcept;

		const physical_unit left() const noexcept { return left_; }
		virtual void set_left( physical_unit value ) noexcept;

		const dialog_unit height() const noexcept { return height_; }
		virtual void set_height( dialog_unit value ) noexcept;

		const dialog_unit width() const noexcept { return width_; }
		virtual void set_width( dialog_unit value ) noexcept;

		const physical_unit bottom() const noexcept { return top_ + static_cast<physical_unit>( std::ceil( scale_factor_.scale_y( height_ ) ) ); }
		const physical_unit right() const noexcept { return left_ + static_cast<physical_unit>( std::ceil( scale_factor_.scale_x( width_ ) ) ); }

		const physical_unit physical_height() const noexcept { return static_cast<physical_unit>( std::ceil( scale_factor_.scale_y( height_ ) ) ); }
		const physical_unit physical_width() const noexcept { return static_cast<physical_unit>( std::ceil( scale_factor_.scale_x( width_ ) ) ); }

	protected:
		DWORD style() const noexcept { return style_; }
		void set_style( DWORD value ) noexcept;

	private:
		const window* parent_;

		bool initialized_;
		HINSTANCE hinstance_;
		HWND hwnd_;

		std::wstring text_, class_name_;
		DWORD style_;

		physical_unit top_, left_;
		dialog_unit height_, width_;
		dpi_scale_factor scale_factor_;

		int32_t tabstop_;
	};

}