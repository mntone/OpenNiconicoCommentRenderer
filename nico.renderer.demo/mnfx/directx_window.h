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
#include "window.h"

namespace mnfx {

	class directx_window
		: public window
	{
	public:
		directx_window();

		virtual HRESULT initialize( HINSTANCE hinstance, std::wstring class_name ) noexcept;
		virtual HRESULT initialize( HINSTANCE hinstance, const window& parent_window, std::wstring class_name ) noexcept;

		virtual HRESULT on_render() noexcept
		{
			return S_OK;
		}

	protected:
		virtual LRESULT window_procedure( window_message message, WPARAM wparam, LPARAM lparam, bool& handled ) noexcept;
		virtual HRESULT on_resize( dialog_unit height, dialog_unit width ) noexcept;
		virtual HRESULT initialize_resources() noexcept;
		virtual HRESULT initialize_device_dependent_resources() noexcept;
		virtual HRESULT initialize_size_dependent_resources() noexcept;
		virtual void release_resources() noexcept;
		virtual void release_device_dependent_resources() noexcept;
		virtual void release_size_dependent_resources() noexcept;
		virtual HRESULT handle_devices_lost() noexcept;

	private:	
		HRESULT initialize( HINSTANCE hinstance, const window* const parent_window, std::wstring class_name ) noexcept;

		HRESULT prepare_render();

	protected:
#if( defined( _WIN32_WINNT ) && _WIN32_WINNT <= _WIN32_WINNT_WIN8 )
		const Microsoft::WRL::ComPtr<ID2D1Factory1>& d2d_factory() const noexcept { return d2d_factory_; }
		const Microsoft::WRL::ComPtr<IDWriteFactory1>& dwrite_factory() const noexcept { return dwrite_factory_; }

		const Microsoft::WRL::ComPtr<ID3D11Device1>& d3d_device() const noexcept{ return d3d_device_; }
		const Microsoft::WRL::ComPtr<ID3D11DeviceContext1>& d3d_device_context() const noexcept{ return d3d_device_context_; }
		const Microsoft::WRL::ComPtr<IDXGIDevice2>& dxgi_device() const noexcept { return dxgi_device_; }
		const Microsoft::WRL::ComPtr<ID2D1Device>& d2d_device() const noexcept { return d2d_device_; }
		const Microsoft::WRL::ComPtr<ID2D1DeviceContext>& d2d_device_context() const noexcept { return d2d_device_context_; }
#else
		const Microsoft::WRL::ComPtr<ID2D1Factory2>& d2d_factory() const noexcept { return d2d_factory_; }
		const Microsoft::WRL::ComPtr<IDWriteFactory2>& dwrite_factory() const noexcept { return dwrite_factory_; }

		const Microsoft::WRL::ComPtr<ID3D11Device2>& d3d_device() const noexcept { return d3d_device_; }
		const Microsoft::WRL::ComPtr<ID3D11DeviceContext2>& d3d_device_context() const noexcept { return d3d_device_context_; }
		const Microsoft::WRL::ComPtr<IDXGIDevice3>& dxgi_device() const noexcept { return dxgi_device_; }
		const Microsoft::WRL::ComPtr<ID2D1Device1>& d2d_device() const noexcept { return d2d_device_; }
		const Microsoft::WRL::ComPtr<ID2D1DeviceContext1>& d2d_device_context() const noexcept { return d2d_device_context_; }
#endif

		const Microsoft::WRL::ComPtr<IDXGIAdapter2>& dxgi_adapter() const noexcept { return dxgi_adapter_; }
		const Microsoft::WRL::ComPtr<IDXGIFactory2>& dxgi_factory() const noexcept { return dxgi_factory_; }
#if( defined( _WIN32_WINNT ) && _WIN32_WINNT <= _WIN32_WINNT_WIN8 )
		const Microsoft::WRL::ComPtr<IDXGISwapChain1>& dxgi_swap_chain() const noexcept { return dxgi_swap_chain_; }
#else
		const Microsoft::WRL::ComPtr<IDXGISwapChain2>& dxgi_swap_chain() const noexcept { return dxgi_swap_chain_; }
#endif
		const Microsoft::WRL::ComPtr<IDXGISurface>& dxgi_surface() const noexcept { return dxgi_surface_; }
		const Microsoft::WRL::ComPtr<ID2D1Bitmap1>& d2d_bitmap() const noexcept { return d2d_bitmap_; }

	private:
		bool initialized_;
		
#if( defined( _WIN32_WINNT ) && _WIN32_WINNT <= _WIN32_WINNT_WIN8 )
		// resources
		Microsoft::WRL::ComPtr<ID2D1Factory1> d2d_factory_;
		Microsoft::WRL::ComPtr<IDWriteFactory1> dwrite_factory_;

		// device-dependent resources
		Microsoft::WRL::ComPtr<ID3D11Device1> d3d_device_;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1> d3d_device_context_;
		Microsoft::WRL::ComPtr<IDXGIDevice2> dxgi_device_;
		Microsoft::WRL::ComPtr<ID2D1Device> d2d_device_;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2d_device_context_;
#else
		// resources
		Microsoft::WRL::ComPtr<ID2D1Factory2> d2d_factory_;
		Microsoft::WRL::ComPtr<IDWriteFactory2> dwrite_factory_;

		// device-dependent resources
		Microsoft::WRL::ComPtr<ID3D11Device2> d3d_device_;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext2> d3d_device_context_;
		Microsoft::WRL::ComPtr<IDXGIDevice3> dxgi_device_;
		Microsoft::WRL::ComPtr<ID2D1Device1> d2d_device_;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext1> d2d_device_context_;
#endif

		// size-dependent resources
		Microsoft::WRL::ComPtr<IDXGIAdapter2> dxgi_adapter_;
		Microsoft::WRL::ComPtr<IDXGIFactory2> dxgi_factory_;
#if( defined( _WIN32_WINNT ) && _WIN32_WINNT <= _WIN32_WINNT_WIN8 )
		Microsoft::WRL::ComPtr<IDXGISwapChain1> dxgi_swap_chain_;
#else
		Microsoft::WRL::ComPtr<IDXGISwapChain2> dxgi_swap_chain_;
#endif
		Microsoft::WRL::ComPtr<IDXGISurface> dxgi_surface_;
		Microsoft::WRL::ComPtr<ID2D1Bitmap1> d2d_bitmap_;
	};

}