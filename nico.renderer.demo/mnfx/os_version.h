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
#include <Windows.h>

#pragma warning( disable: 4996 )

namespace mnfx {

	class os_version
	{
	public:
		enum os_label { before = 60, seven, eight, eight_one, later };

		os_version()
		{
			OSVERSIONINFOW osVersion = { sizeof( OSVERSIONINFOW ) };
			GetVersionExW( &osVersion );

			if( osVersion.dwMajorVersion > 6 )
			{
				os_version_label_ = os_label::later;
			}
			else
			{
				switch( osVersion.dwMinorVersion )
				{
				case 3:
					os_version_label_ = os_label::eight_one;
					break;
				case 2:
					os_version_label_ = os_label::eight;
					break;
				case 1:
					os_version_label_ = os_label::seven;
					break;
				default:
					os_version_label_ = os_label::before;
					break;
				}
			}
		}

		bool operator==( const os_label& rhs ) const noexcept { return this->os_version_label_ == rhs; }
		bool operator!=( const os_label& rhs ) const noexcept { return this->os_version_label_ != rhs; }
		bool operator>( const os_label& rhs ) const noexcept { return this->os_version_label_ > rhs; }
		bool operator<( const os_label& rhs ) const noexcept { return this->os_version_label_ < rhs; }
		bool operator>=( const os_label& rhs ) const noexcept { return this->os_version_label_ >= rhs; }
		bool operator<=( const os_label& rhs ) const noexcept { return this->os_version_label_ <= rhs; }

		bool operator==( const os_version& rhs ) const noexcept { return this->os_version_label_ == rhs.os_version_label_; }
		bool operator!=( const os_version& rhs ) const noexcept { return this->os_version_label_ != rhs.os_version_label_; }
		bool operator>( const os_version& rhs ) const noexcept { return this->os_version_label_ > rhs.os_version_label_; }
		bool operator<( const os_version& rhs ) const noexcept { return this->os_version_label_ < rhs.os_version_label_; }
		bool operator>=( const os_version& rhs ) const noexcept { return this->os_version_label_ >= rhs.os_version_label_; }
		bool operator<=( const os_version& rhs ) const noexcept { return this->os_version_label_ <= rhs.os_version_label_; }

		os_label Get() const { return os_version_label_; }

	private:
		os_label os_version_label_;
	};

}