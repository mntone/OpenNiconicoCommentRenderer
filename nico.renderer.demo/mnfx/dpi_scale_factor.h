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

#pragma once
#include "os_version.h"

namespace mnfx {


	class dpi_scale_factor
	{
	public:
		using dpi_unit = float;

		dpi_scale_factor()
		{
			if( osVersion >= os_version::eight_one )
			{
				hdllinstance = LoadLibrary( L"shcore.dll" );
				fnGetDpiForMonitor = reinterpret_cast<GetDpiForMonitor*>( GetProcAddress( hdllinstance, "GetDpiForMonitor" ) );
			}
		}

		virtual ~dpi_scale_factor()
		{
			if( osVersion >= os_version::eight_one )
			{
				FreeLibrary( hdllinstance );
			}
		}

		void initialize( HMONITOR hmonitor )
		{
			if( osVersion >= os_version::eight_one )
			{
				UINT y, x;
				fnGetDpiForMonitor( hmonitor, 0, &x, &y );
				ydpi = static_cast<dpi_unit>( y ) / 96;
				xdpi = static_cast<dpi_unit>( x ) / 96;
			}
			else
			{
				HDC hdc = GetDC( nullptr );
				ydpi = static_cast<dpi_unit>( GetDeviceCaps( hdc, LOGPIXELSY ) ) / 96;
				xdpi = static_cast<dpi_unit>( GetDeviceCaps( hdc, LOGPIXELSX ) ) / 96;
				ReleaseDC( nullptr, hdc );
			}
		}

		std::pair<dpi_unit, dpi_unit> set_dpi( uint16_t y, uint16_t x )
		{
			dpi_unit new_ydpi, new_xdpi;
			new_ydpi = static_cast<dpi_unit>( y ) / 96;
			new_xdpi = static_cast<dpi_unit>( x ) / 96;

			float scale_factor_y, scale_factor_x;
			scale_factor_y = new_ydpi / ydpi;
			scale_factor_x = new_xdpi / xdpi;

			ydpi = new_ydpi;
			xdpi = new_xdpi;

			return std::make_pair( scale_factor_y, scale_factor_x );
		}

		template<typename T>
		T scale_y( T virtual_y ) const
		{
			return static_cast<T>( ydpi * static_cast<dpi_unit>( virtual_y ) );
		}

		template<typename T>
		T scale_x( T virtual_x ) const
		{
			return static_cast<T>( xdpi * static_cast<dpi_unit>( virtual_x ) );
		}

		template<typename T>
		T scale_inverse_y( T physical_y ) const
		{
			return static_cast<T>( static_cast<dpi_unit>( physical_y ) / ydpi );
		}

		template<typename T>
		T scale_inverse_x( T physical_x ) const
		{
			return static_cast<T>( static_cast<dpi_unit>( physical_x ) / xdpi );
		}

	private:
		HINSTANCE hdllinstance;
		dpi_unit ydpi, xdpi;
		os_version osVersion;

		typedef HRESULT __stdcall GetDpiForMonitor( HMONITOR hmonitor, int dpiType, UINT* dpiX, UINT* dpiY );
		GetDpiForMonitor* fnGetDpiForMonitor;
	};

}