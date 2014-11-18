#include "pch.h"
#include "windows/directx_renderer_driver.h"
#include "rendering_comment.h"

struct directx_extra_data
{
	IDWriteTextLayout1* layout;
	ID2D1Image* image;
};

using namespace D2D1;
using namespace Microsoft::WRL;
using namespace nico::renderer;
using namespace nico::renderer::windows;

const auto space = 3.f;
const auto twiceSpace = 2.f * space;

directx_renderer_driver::directx_renderer_driver()
{
	FILETIME ft;
	GetSystemTimeAsFileTime( &ft );
	base_time_ = *reinterpret_cast<int64_t*>( &ft );
}

HRESULT directx_renderer_driver::initialize( ID2D1DeviceContext* const d2d_device_context ) noexcept
{
	HRESULT hr = S_OK;

	d2d_device_context_ = d2d_device_context;

	hr = d2d_device_context_->CreateSolidColorBrush(
		ColorF( ColorF::Yellow ),
		yellow_solid_color_brush_.GetAddressOf() );
	if( FAILED( hr ) )
	{
		return hr;
	}

	hr = d2d_device_context_->CreateSolidColorBrush(
		ColorF( ColorF::White ),
		foreground_brush_.GetAddressOf() );
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
	const auto& now_time = *reinterpret_cast<int64_t*>( &ft );
	return std::chrono::nanoseconds( 100 * ( now_time - base_time_ ) );
}

comment_text_info directx_renderer_driver::text_info( rendering_comment& comment ) const noexcept
{
	HRESULT hr = S_OK;

	ComPtr<IDWriteTextLayout1> text_layout;
	hr = create_layout( comment, text_layout.GetAddressOf() );

	DWRITE_TEXT_METRICS metrics;
	text_layout->GetMetrics( &metrics );

	comment_text_info info = { metrics.height, metrics.width };
	return info;
}

void directx_renderer_driver::render( const std::deque<rendering_comment*>& comments ) noexcept
{
	auto func = [=]( rendering_comment* const comment )
	{
		HRESULT hr = S_OK;

		auto& extra_data = comment->extra_data<directx_extra_data>();
		if( size_changed_ || extra_data.image == nullptr )
		{
			if( size_changed_ )
			{
				if( extra_data.image != nullptr )
				{
					extra_data.image->Release();
					extra_data.image = nullptr;
				}
				if( extra_data.layout != nullptr )
				{
					extra_data.layout->Release();
					extra_data.layout = nullptr;
				}
			}

			ComPtr<IDWriteTextLayout1> text_layout;
			create_layout( *comment, text_layout.GetAddressOf() );

			ComPtr<ID2D1SolidColorBrush> brush;
			{
				foreground_brush_->SetColor( ColorF( comment->color(), comment->alpha() ) );
				hr = foreground_brush_.As( &brush );
				if( FAILED( hr ) )
				{
					return;
				}
			}

			ComPtr<ID2D1BitmapRenderTarget> bitmap_render_target;
			hr = d2d_device_context_->CreateCompatibleRenderTarget(
				SizeF( comment->width() + twiceSpace, comment->height() + twiceSpace ),
				bitmap_render_target.GetAddressOf() );
			if( FAILED( hr ) )
			{
				return;
			}

			bitmap_render_target->BeginDraw();
			bitmap_render_target->Clear( ColorF( 0, 0.f ) );
			if( comment->self() )
			{
				bitmap_render_target->DrawRectangle(
					RectF( space - 1.f, space, comment->width() + space + 2.f, comment->height() + space + 1.f ),
					yellow_solid_color_brush_.Get() );
			}
			bitmap_render_target->DrawTextLayout(
				Point2F( space, space ),
				text_layout.Get(),
				brush.Get(),
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
				static_cast<D2D1_DRAW_TEXT_OPTIONS>( 0x00000004 ) );
#else
				D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT );
#endif
			bitmap_render_target->EndDraw();

			ComPtr<ID2D1Bitmap> d2d_bitmap;
			bitmap_render_target->GetBitmap( d2d_bitmap.GetAddressOf() );


			ComPtr<ID2D1Effect> shadow_effect;
			d2d_device_context_->CreateEffect( CLSID_D2D1Shadow, shadow_effect.GetAddressOf() );
			shadow_effect->SetInput( 0, d2d_bitmap.Get() );
			shadow_effect->SetValue( D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, 1.f );
			shadow_effect->SetValue( D2D1_SHADOW_PROP_COLOR, Vector4F( 0.f, 0.f, 0.f, 1.f ) );
			shadow_effect->SetValue( D2D1_SHADOW_PROP_OPTIMIZATION, D2D1_DIRECTIONALBLUR_OPTIMIZATION_QUALITY );

			ComPtr<ID2D1Effect> affine_transform_effect;
			d2d_device_context_->CreateEffect( CLSID_D2D12DAffineTransform, affine_transform_effect.GetAddressOf() );
			affine_transform_effect->SetInputEffect( 0, shadow_effect.Get() );
			affine_transform_effect->SetValue( D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, Matrix3x2F::Translation( 0.7f, 0.7f ) );


			ComPtr<ID2D1Effect> shadow_effect2;
			d2d_device_context_->CreateEffect( CLSID_D2D1Shadow, shadow_effect2.GetAddressOf() );
			shadow_effect2->SetInput( 0, d2d_bitmap.Get() );
			shadow_effect2->SetValue( D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, 1.f );
			if( comment->color() == 0 )
			{
				shadow_effect2->SetValue( D2D1_SHADOW_PROP_COLOR, Vector4F( 1.f, 1.f, 1.f, 1.f ) );
			}
			else
			{
				shadow_effect2->SetValue( D2D1_SHADOW_PROP_COLOR, Vector4F( 0.f, 0.f, 0.f, 1.f ) );
			}
			shadow_effect2->SetValue( D2D1_SHADOW_PROP_OPTIMIZATION, D2D1_DIRECTIONALBLUR_OPTIMIZATION_QUALITY );

			ComPtr<ID2D1Effect> affine_transform_effect2;
			d2d_device_context_->CreateEffect( CLSID_D2D12DAffineTransform, affine_transform_effect2.GetAddressOf() );
			affine_transform_effect2->SetInputEffect( 0, shadow_effect2.Get() );
			affine_transform_effect2->SetValue( D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, Matrix3x2F::Translation( -0.7f, -0.7f ) );


			ComPtr<ID2D1Effect> composite_effect;
			d2d_device_context_->CreateEffect( CLSID_D2D1Composite, &composite_effect );
			composite_effect->SetInputEffect( 0, affine_transform_effect.Get() );
			composite_effect->SetInputEffect( 1, affine_transform_effect2.Get() );

			ComPtr<ID2D1Effect> composite_effect2;
			d2d_device_context_->CreateEffect( CLSID_D2D1Composite, &composite_effect2 );
			composite_effect2->SetValue( D2D1_COMPOSITE_PROP_MODE, D2D1_COMPOSITE_MODE_DESTINATION_OVER );
			composite_effect2->SetInput( 0, d2d_bitmap.Get() );
			composite_effect2->SetInputEffect( 1, composite_effect.Get() );

			ComPtr<ID2D1Image> d2d_image;
			composite_effect2->GetOutput( d2d_image.GetAddressOf() );

			d2d_image.CopyTo( &extra_data.image );
		}

		d2d_device_context_->DrawImage(
			extra_data.image,
			Point2F( comment->left() - space, comment->top() - space ),
			RectF( 0.f, 0.f, comment->width() + twiceSpace, comment->height() + twiceSpace ) );
	};

	for( const auto& comment : comments )
	{
		if( comment->alpha() != 1.f && comment->is_center() )
		{
			func( comment );
		}
	}
	for( const auto& comment : comments )
	{
		if( comment->alpha() == 1.f && comment->is_center() )
		{
			func( comment );
		}
	}
	for( const auto& comment : comments )
	{
		if( comment->is_not_center() )
		{
			func( comment );
		}
	}

	size_changed_ = false;
}

HRESULT directx_renderer_driver::create_layout( rendering_comment& comment, IDWriteTextLayout1** layout ) const noexcept
{
	HRESULT hr = S_OK;

	auto& extra_data = comment.extra_data<directx_extra_data>();
	if( extra_data.layout != nullptr )
	{
		*layout = extra_data.layout;
		return hr;
	}

	ComPtr<IDWriteTextFormat> text_format;
	if( comment.font_size() == 39.f )
	{
		hr = large_text_format_.As( &text_format );
	}
	else if( comment.font_size() == 24.f )
	{
		hr = medium_text_format_.As( &text_format );
	}
	else if( comment.font_size() == 15.f )
	{
		hr = small_text_format_.As( &text_format );
	}
	else
	{
		hr = create_font( comment.font_size(), text_format.GetAddressOf() );
	}
	if( FAILED( hr ) )
	{
		return hr;
	}

	ComPtr<IDWriteTextLayout> text_layout;
	dwrite_factory_->CreateTextLayout(
		comment.value(),
		static_cast<UINT32>( comment.length() ),
		text_format.Get(),
		FLT_MAX,
		FLT_MAX,
		text_layout.GetAddressOf() );

	auto begin = comment.analysis_data()[0].begin;
	for( auto&& group : comment.analysis_data() )
	{
		DWRITE_TEXT_RANGE range = { group.begin - begin, group.end - group.begin + 1 };
		if( group.font == comment_font_type::ms_p_gothic )
		{
#if WINAPI_FAMILY != WINAPI_FAMILY_PHONE_APP
			text_layout->SetFontFamilyName( L"‚l‚r ‚oƒSƒVƒbƒN", range );
#endif
		}
		else if( group.font == comment_font_type::sim_sun )
		{
			text_layout->SetFontFamilyName( L"SimSun", range );
		}
		else if( group.font == comment_font_type::gulim )
		{
#if WINAPI_FAMILY != WINAPI_FAMILY_PHONE_APP
			text_layout->SetFontFamilyName( L"Gulim", range );
#endif
		}
		else if( group.font == comment_font_type::p_ming_li_u )
		{
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
			text_layout->SetFontFamilyName( L"SimSun", range );
#else
			text_layout->SetFontFamilyName( L"PMingLiU", range );
#endif
		}
	}

	auto line_height = 1.34f * comment.font_size();
	text_layout->SetLineSpacing( DWRITE_LINE_SPACING_METHOD_UNIFORM, line_height, 0.8f * line_height );

	text_layout.CopyTo( &extra_data.layout );
	extra_data.layout->AddRef();

	text_layout.CopyTo( IID_PPV_ARGS( layout ) );

	return hr;
}

HRESULT directx_renderer_driver::create_font( const float font_size, IDWriteTextFormat** text_format ) const noexcept
{
	return dwrite_factory_->CreateTextFormat(
	L"Arial",
		nullptr,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		font_size,
		L"ja-JP",
		text_format );
}

void* directx_renderer_driver::initialize_extra_data() const noexcept
{
	return new directx_extra_data();
}

void directx_renderer_driver::release_extra_data( void* ptr ) const noexcept
{
	auto& data = *reinterpret_cast<directx_extra_data*>( ptr );
	if( data.image != nullptr )
	{
		data.image->Release();
		data.image = nullptr;
	}
	if( data.layout != nullptr )
	{
		data.layout->Release();
		data.layout = nullptr;
	}
	delete &data;
}