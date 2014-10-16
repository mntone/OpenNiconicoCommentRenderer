#pragma once
#include "comment_base.h"

namespace nico { namespace renderer { namespace windows {

	class directx_renderer_driver final
		: private renderer_driver
	{
	public:
		NICORIMP directx_renderer_driver();

		NICORIMP HRESULT initialize( ID2D1DeviceContext* const d2d_device_context ) noexcept;

		virtual comment_time now() const noexcept;
		virtual comment_text_info text_info( rendering_comment& comment ) const noexcept;
		virtual void render( const std::deque<rendering_comment*>& comments ) noexcept;

		virtual void* initialize_extra_data() const noexcept;
		virtual void release_extra_data( void* ptr ) const noexcept;

	private:
		HRESULT create_layout( rendering_comment& comment, IDWriteTextLayout1** layout ) const noexcept;
		HRESULT create_font( const float font_size, IDWriteTextFormat** text_format ) const noexcept;

	public:
		void set_dwrite_factory( IDWriteFactory1* const value ) noexcept { dwrite_factory_ = value; }

		void size_change() noexcept { size_changed_ = true; }

	private:
		bool size_changed_;
		int64_t base_time_;

		::Microsoft::WRL::ComPtr<IDWriteFactory1> dwrite_factory_;
		::Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2d_device_context_;

		::Microsoft::WRL::ComPtr<IDWriteTextFormat> large_text_format_, medium_text_format_, small_text_format_;
		::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> yellow_solid_color_brush_;
		::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> foreground_brush_;
	};

} } }