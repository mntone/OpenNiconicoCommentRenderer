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
#include "directx_window.h"

using namespace mnfx;

directx_window::directx_window()
	: initialized_( false )
	, d2d_factory_( nullptr )
	, dwrite_factory_( nullptr )
	, d3d_device_( nullptr )
	, d3d_device_context_( nullptr )
	, d2d_device_( nullptr )
	, d2d_device_context_( nullptr )
	, dxgi_swap_chain_( nullptr )
	, d2d_bitmap_( nullptr )
{ }

HRESULT directx_window::initialize( HINSTANCE hinstance, std::wstring class_name ) noexcept
{
	return initialize( std::move( hinstance ), nullptr, std::move( class_name ) );
}

HRESULT directx_window::initialize( HINSTANCE hinstance, const window& parent_window, std::wstring class_name ) noexcept
{
	return initialize( std::move( hinstance ), &parent_window, std::move( class_name ) );
}

HRESULT directx_window::initialize( HINSTANCE hinstance, const window* const parent_window, std::wstring class_name ) noexcept
{
	HRESULT hr = S_OK;

	hr = window::initialize( std::move( hinstance ), std::move( parent_window ), std::move( class_name ) );
	if( FAILED( hr ) )
	{
		return hr;
	}

	hr = initialize_resources();
	if( FAILED( hr ) )
	{
		return hr;
	}

	hr = initialize_device_dependent_resources();
	if( FAILED( hr ) )
	{
		return hr;
	}
	initialized_ = true;

	hr = initialize_size_dependent_resources();

	return hr;
}

LRESULT directx_window::window_procedure( window_message message, WPARAM wparam, LPARAM lparam, bool& handled ) noexcept
{
	switch( message )
	{
	case window_message::paint:
		prepare_render();
		handled = true;
		return 0;
	}
	return window::window_procedure( message, wparam, lparam, handled );
}

HRESULT directx_window::on_resize( dialog_unit height, dialog_unit width ) noexcept
{
	HRESULT hr = S_OK;

	hr = window::on_resize( height, width );
	if( FAILED( hr ) )
	{
		return hr;
	}
	
	if( initialized_ )
	{
		release_size_dependent_resources();
		hr = initialize_size_dependent_resources();
	}

	return hr;
}

HRESULT directx_window::initialize_resources() noexcept
{
	HRESULT hr = S_OK;

	hr = D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, d2d_factory_.GetAddressOf() );
	if( FAILED( hr ) )
	{
		return hr;
	}

#if( defined( _WIN32_WINNT ) && _WIN32_WINNT <= _WIN32_WINNT_WIN8 )
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof( IDWriteFactory1 ),
		reinterpret_cast<IUnknown**>( dwrite_factory_.GetAddressOf() ) );
#else
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof( IDWriteFactory2 ),
		reinterpret_cast<IUnknown**>( dwrite_factory_.GetAddressOf() ) );
#endif

	if( FAILED( hr ) )
	{
		return hr;
	}

	return hr;
}

HRESULT directx_window::initialize_device_dependent_resources() noexcept
{
	HRESULT hr = S_OK;

	UINT creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	const D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	Microsoft::WRL::ComPtr<ID3D11Device> d3d_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3d_device_context;
	hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creation_flags,
		feature_levels,
		ARRAYSIZE( feature_levels ),
		D3D11_SDK_VERSION,
		d3d_device.GetAddressOf(),
		nullptr,
		d3d_device_context.GetAddressOf() );
	if( FAILED( hr ) )
	{
		return hr;
	}

	hr = d3d_device.As( &d3d_device_ );
	if( FAILED( hr ) )
	{
		return hr;
	}

	hr = d3d_device_context.As( &d3d_device_context_ );
	if( FAILED( hr ) )
	{
		return hr;
	}

#if( defined( _WIN32_WINNT ) && _WIN32_WINNT <= _WIN32_WINNT_WIN8 )
	ComPtr<IDXGIDevice2> dxgi_device;
#else
	ComPtr<IDXGIDevice3> dxgi_device;
#endif
	hr = d3d_device_.As( &dxgi_device );
	if( FAILED( hr ) )
	{
		return hr;
	}

	hr = dxgi_device->SetMaximumFrameLatency( 1 );
	if( FAILED( hr ) )
	{
		return hr;
	}

	hr = d2d_factory_->CreateDevice( dxgi_device.Get(), d2d_device_.GetAddressOf() );
	if( FAILED( hr ) )
	{
		return hr;
	}

	hr = d2d_device_->CreateDeviceContext( D2D1_DEVICE_CONTEXT_OPTIONS_NONE, d2d_device_context_.GetAddressOf() );
	if( FAILED( hr ) )
	{
		return hr;
	}

	d2d_device_context_->SetAntialiasMode( D2D1_ANTIALIAS_MODE_PER_PRIMITIVE );
	d2d_device_context_->SetTextAntialiasMode( D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE );

	return hr;
}

HRESULT directx_window::initialize_size_dependent_resources() noexcept
{
	HRESULT hr = S_OK;

	if( dxgi_swap_chain_ != nullptr )
	{
		hr = dxgi_swap_chain_->ResizeBuffers( 2, physical_width(), physical_height(), DXGI_FORMAT_B8G8R8A8_UNORM, 0 );
		if( hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET )
		{
			hr = handle_devices_lost();
			return hr;
		}
	}
	else
	{
#if( defined( _WIN32_WINNT ) && _WIN32_WINNT <= _WIN32_WINNT_WIN8 )
		ComPtr<IDXGIDevice2> dxgi_device;
#else
		ComPtr<IDXGIDevice3> dxgi_device;
#endif
		hr = d3d_device_.As( &dxgi_device );
		if( FAILED( hr ) )
		{
			return hr;
		}

		ComPtr<IDXGIAdapter> dxgi_adapter;
		hr = dxgi_device->GetAdapter( dxgi_adapter.GetAddressOf() );
		if( FAILED( hr ) )
		{
			return hr;
		}

		ComPtr<IDXGIFactory2> dxgi_factory;
		hr = dxgi_adapter->GetParent( IID_PPV_ARGS( dxgi_factory.GetAddressOf() ) );
		if( FAILED( hr ) )
		{
			return hr;
		}

		DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = { 0 };
		swap_chain_desc.Width = physical_width();
		swap_chain_desc.Height = physical_height();
		swap_chain_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swap_chain_desc.Stereo = false;
		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.SampleDesc.Quality = 0;
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.BufferCount = 2;
		swap_chain_desc.Scaling = DXGI_SCALING_NONE;
		swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		swap_chain_desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		swap_chain_desc.Flags = 0;

		ComPtr<IDXGISwapChain1> dxgi_swap_chain;
		hr = dxgi_factory->CreateSwapChainForHwnd(
			d3d_device_.Get(),
			hwnd(),
			&swap_chain_desc,
			nullptr,
			nullptr,
			dxgi_swap_chain.GetAddressOf() );
		if( FAILED( hr ) )
		{
			return hr;
		}

		hr = dxgi_swap_chain.As( &dxgi_swap_chain_ );
	}
	if( FAILED( hr ) )
	{
		return hr;
	}

	ComPtr<IDXGISurface> dxgi_back_buffer;
	hr = dxgi_swap_chain_->GetBuffer( 0, IID_PPV_ARGS( dxgi_back_buffer.ReleaseAndGetAddressOf() ) );
	if( FAILED( hr ) )
	{
		return hr;
	}

	D2D1_BITMAP_PROPERTIES1 bitmap_properties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat( DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED ),
		96.0f,
		96.0f );

	hr = d2d_device_context_->CreateBitmapFromDxgiSurface( dxgi_back_buffer.Get(), &bitmap_properties, d2d_bitmap_.ReleaseAndGetAddressOf() );
	if( FAILED( hr ) )
	{
		return hr;
	}

	d2d_device_context_->SetTarget( d2d_bitmap_.Get() );

	return hr;
}

void directx_window::release_resources() noexcept
{
	release_device_dependent_resources();
	dwrite_factory_.Reset();
	d2d_factory_.Reset();
}

void directx_window::release_device_dependent_resources() noexcept
{
	release_size_dependent_resources();
	d2d_device_context_.Reset();
	d2d_device_.Reset();
	d3d_device_context_.Reset();
	d3d_device_.Reset();
}

void directx_window::release_size_dependent_resources() noexcept
{
	d2d_device_context_->SetTarget( nullptr );
	d3d_device_context_->OMSetRenderTargets( 0, nullptr, nullptr );
	d3d_device_context_->Flush();

	d2d_bitmap_.Reset();
	dxgi_swap_chain_.Reset();
}

HRESULT directx_window::handle_devices_lost() noexcept
{
	HRESULT hr = S_OK;

	release_device_dependent_resources();

	hr = initialize_device_dependent_resources();
	if( FAILED( hr ) )
	{
		return hr;
	}

	hr = initialize_size_dependent_resources();

	return hr;
}

HRESULT directx_window::prepare_render()
{
	HRESULT hr = S_OK;

	if( !initialized_ )
	{
		return hr;
	}

	{
		hr = on_render();
		if( FAILED( hr ) )
		{
			return hr;
		}

		DXGI_PRESENT_PARAMETERS parameters = { 0 };
		parameters.DirtyRectsCount = 0;
		parameters.pDirtyRects = nullptr;
		parameters.pScrollRect = nullptr;
		parameters.pScrollOffset = nullptr;
		hr = dxgi_swap_chain_->Present1( 1, 0, &parameters );
	}

	if( hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET )
	{
		hr = handle_devices_lost();
	}
	return hr;
}