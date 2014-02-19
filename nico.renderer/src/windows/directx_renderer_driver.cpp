#include "pch.h"
#include "windows/directx_renderer_driver.h"
#include "rendering_comment.h"

using namespace Microsoft::WRL;
using namespace nico::renderer;
using namespace nico::renderer::windows;

directx_renderer_driver::directx_renderer_driver()
{
	FILETIME ft;
	GetSystemTimeAsFileTime( &ft );
	const auto& li = *reinterpret_cast<LARGE_INTEGER*>( &ft );
	base_time_ = li.QuadPart;
}

HRESULT directx_renderer_driver::initialize( ID2D1DeviceContext* const d2d_device_context ) noexcept
{
	HRESULT hr = S_OK;

	d2d_device_context_ = d2d_device_context;

	hr = d2d_device_context_->CreateSolidColorBrush(
		D2D1::ColorF( D2D1::ColorF::White ),
		white_solid_color_brush_.GetAddressOf() );
	if( FAILED( hr ) )
	{
		return hr;
	}

	hr = d2d_device_context_->CreateSolidColorBrush(
		D2D1::ColorF( D2D1::ColorF::Yellow ),
		yellow_solid_color_brush_.GetAddressOf() );
	if( FAILED( hr ) )
	{
		return hr;
	}

	hr = create_font( 39.0f, large_text_format_.GetAddressOf() );
	if( FAILED( hr ) )
	{
		return hr;
	}

	hr = create_font( 24.0f, medium_text_format_.GetAddressOf() );
	if( FAILED( hr ) )
	{
		return hr;
	}

	hr = create_font( 15.0f, small_text_format_.GetAddressOf() );

	return hr;
}

comment_time directx_renderer_driver::now() const noexcept
{
	FILETIME ft;
	GetSystemTimeAsFileTime( &ft );
	const auto& li = *reinterpret_cast<LARGE_INTEGER*>( &ft );
	return std::chrono::nanoseconds( 100 * ( li.QuadPart - base_time_ ) );
}

comment_text_info directx_renderer_driver::text_info( comment_position font_size, const wchar_t* text ) const noexcept
{
	HRESULT hr = S_OK;

	ComPtr<IDWriteTextFormat> text_format;
	if( font_size == 39.0f )
	{
		hr = large_text_format_.As( &text_format );
	}
	else if( font_size == 24.0f )
	{
		hr = medium_text_format_.As( &text_format );
	}
	else if( font_size == 15.0f )
	{
		hr = small_text_format_.As( &text_format );
	}
	else
	{
		hr = create_font( font_size, text_format.GetAddressOf() );
	}

	ComPtr<IDWriteTextLayout> text_layout;
	dwrite_factory_->CreateTextLayout(
		text,
		static_cast<UINT32>( wcslen( text ) ),
		text_format.Get(),
		FLT_MAX,
		FLT_MAX,
		text_layout.GetAddressOf() );

	DWRITE_TEXT_METRICS metrics;
	text_layout->GetMetrics( &metrics );

	comment_text_info info = { metrics.height, metrics.width };
	return info;
}

void directx_renderer_driver::render( const std::deque<rendering_comment*>& comments ) noexcept
{
	HRESULT hr = S_OK;

	d2d_device_context_->BeginDraw();
	d2d_device_context_->Clear( D2D1::ColorF( D2D1::ColorF::Black ) );

	for( const auto& comment : comments )
	{
		if( comment->self() )
		{
			d2d_device_context_->DrawRectangle(
				D2D1::RectF( comment->left() - 1.0f, comment->top(), comment->right() + 1.0f, comment->bottom() + 1.0f ),
				yellow_solid_color_brush_.Get() );
		}

		ComPtr<IDWriteTextFormat> text_format;
		if( comment->font_size() == 39.0f )
		{
			hr = large_text_format_.As( &text_format );
		}
		else if( comment->font_size() == 24.0f )
		{
			hr = medium_text_format_.As( &text_format );
		}
		else if( comment->font_size() == 15.0f )
		{
			hr = small_text_format_.As( &text_format );
		}
		else
		{
			hr = create_font( comment->font_size(), text_format.GetAddressOf() );
		}

		ComPtr<IDWriteTextLayout> text_layout;
		dwrite_factory_->CreateTextLayout(
			comment->value(),
			static_cast<UINT32>( wcslen( comment->value() ) ),
			text_format.Get(),
			FLT_MAX,
			FLT_MAX,
			text_layout.GetAddressOf() );

		ComPtr<ID2D1SolidColorBrush> brush;
		if( comment->color() == 0xffffff && comment->alpha() == 1.0f )
		{
			hr = white_solid_color_brush_.As( &brush );
			if( FAILED( hr ) )
			{
				return;
			}
		}
		else
		{
			hr = d2d_device_context_->CreateSolidColorBrush(
				D2D1::ColorF( comment->color(), comment->alpha() ),
				brush.GetAddressOf() );
			if( FAILED( hr ) )
			{
				return;
			}
		}

		d2d_device_context_->DrawTextLayout(
			D2D1::Point2F( comment->left(), comment->top() ),
			text_layout.Get(),
			brush.Get(),
			D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT );
	}

	d2d_device_context_->EndDraw();
}

HRESULT directx_renderer_driver::create_font( const float font_size, IDWriteTextFormat** text_format ) const noexcept
{
	return dwrite_factory_->CreateTextFormat(
		L"‚l‚r ‚oƒSƒVƒbƒN",
		nullptr,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		font_size,
		L"ja-JP",
		text_format );
}