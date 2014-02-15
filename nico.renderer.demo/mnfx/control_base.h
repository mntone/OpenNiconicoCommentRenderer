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
#include <cstdint>
#include <string>
#include <Windows.h>

namespace mnfx {

	using dialog_unit = float;
	class window;

	const dialog_unit default_top = 11;
	const dialog_unit default_left = 11;
	const dialog_unit default_height = 26;
	const dialog_unit default_one_line_height = 16;
	const dialog_unit default_width = 90;

	class control_base
	{
		friend class edit_box;

	public:
		control_base();
		virtual ~control_base();

	public:
		virtual HRESULT initialize( const window& parent, LPCWSTR class_name, int32_t tabstop ) noexcept;

		const dialog_unit bottom() const noexcept { return top_ + height_; }
		const dialog_unit right() const noexcept { return left_ + width_; }
		
		bool is_self( const HWND hwnd ) const noexcept { return hwnd == hwnd_; }
			
	private:
		HRESULT set_text() noexcept;
		HRESULT set_position_and_size() noexcept;
		HRESULT set_position() noexcept;
		HRESULT set_size() noexcept;
		HRESULT set_style() noexcept;
		void set_enable() noexcept;

	public:
		const window* const parent() const noexcept { return parent_; }

		const HWND& hwnd() const noexcept { return hwnd_; }

		virtual const std::wstring& text() const noexcept { return text_; }
		void set_text( std::wstring value ) noexcept;

		const dialog_unit top() const noexcept { return top_; }
		void set_top( dialog_unit value ) noexcept;

		const dialog_unit left() const noexcept { return left_; }
		void set_left( dialog_unit value ) noexcept;

		const dialog_unit height() const noexcept { return height_; }
		void set_height( dialog_unit value ) noexcept;

		const dialog_unit width() const noexcept { return width_; }
		void set_width( dialog_unit value ) noexcept;

		DWORD style() const noexcept { return style_; }
		void set_style( DWORD value ) noexcept;

		bool enable() const noexcept { return enable_; }
		void set_enable( bool value ) noexcept;

	protected:
		bool initialized() const noexcept { return initialized_; }

	private:
		static NONCLIENTMETRICSW non_client_metrics_;
		static HFONT gui_font_;

		const window* parent_;
		bool initialized_, enable_;
		HWND hwnd_;
		mutable std::wstring text_;
		DWORD style_;
		dialog_unit top_, left_, height_, width_;
	};

	class button
		: public control_base
	{
	public:
		button()
		{
			set_style( style() | BS_PUSHBUTTON );
		}

		virtual HRESULT initialize( const window& parent, int32_t tabstop ) noexcept
		{
			return control_base::initialize( parent, L"BUTTON", tabstop );
		}
	};

	class radio_button final
		: public button
	{
	public:
		radio_button()
			: checked_( false )
		{
			set_style( ( style() | BS_AUTORADIOBUTTON ) & ~BS_PUSHBUTTON );
		}

		HRESULT initialize( const window& parent, int32_t tabstop ) noexcept
		{
			HRESULT hr = S_OK;

			hr = button::initialize( parent, tabstop );
			if( FAILED( hr ) )
			{
				return hr;
			}

			set_checked();

			return hr;
		}

	private:
		void set_checked() noexcept;

	public:
		bool checked() const noexcept { return checked_; }
		void set_checked( bool value ) noexcept;

	private:
		bool checked_;
	};

	class edit_box final
		: public control_base
	{
	public:
		edit_box()
		{
			set_width( 100 );
			set_style( style() | ES_LEFT | ES_AUTOHSCROLL | WS_BORDER );
		}

		HRESULT initialize( const window& parent, int32_t tabstop ) noexcept
		{
			return control_base::initialize( parent, L"EDIT", tabstop );
		}

	public:
		const std::wstring& text() const noexcept;
	};

}
