#pragma once
#include "comment_base.h"
#if _WINRT_DLL
#include "IComment.h"
#include "native\comment_wrapper.h"
#endif

namespace nico { namespace renderer {

	class rendering_comment final
	{
		friend class renderer;

	public:
		rendering_comment()
			: comment_( nullptr )
		{ }
		rendering_comment( const rendering_comment& ) = delete;
		rendering_comment( rendering_comment&& ) = delete;

		rendering_comment& operator=( const rendering_comment& ) = delete;
		rendering_comment& operator=( rendering_comment&& ) = delete;

	public:
#if _WINRT_DLL
		void set( ::Mntone::Nico::Renderer::IComment^ comment ) noexcept
		{
			comment_ = ::std::make_unique<comment_wrapper>( comment );
#else
		void set( const comment_base& comment ) noexcept
		{
			comment_ = &comment;
#endif
			font_size_ = static_cast<comment_position>( comment_->size() );
			alpha_ = default_comment_alpha;
			paused_ = false;
		}

		void pause() noexcept { paused_ = true; }
		void resume() noexcept { paused_ = false; }

		bool is_not_center() const noexcept { return comment_->vertical_position() != comment_vertical_position_type::center; };
		bool is_center() const noexcept { return comment_->vertical_position() == comment_vertical_position_type::center; };
		bool is_bottom() const noexcept { return comment_->vertical_position() == comment_vertical_position_type::bottom; };

		comment_position bottom() const noexcept { return top_ + height_; }
		comment_position right() const noexcept { return left_ + width_; }

	public:
		const wchar_t* value() const noexcept { return comment_->value(); }
		size_t length() const noexcept { return comment_->length(); }
		bool self() const noexcept { return comment_->self(); }
		comment_vertical_position_type vertical_position() const noexcept { return comment_->vertical_position(); };
		comment_color color() const noexcept { return comment_->color(); }

		bool paused() const noexcept { return paused_; }

		comment_position font_size() const noexcept { return font_size_; }
		void set_font_size( comment_position value ) noexcept { font_size_ = std::move( value ); }
		comment_alpha alpha() const noexcept { return alpha_; }
		void set_alpha( comment_alpha value ) noexcept { alpha_ = std::move( value ); }

		comment_time begin_time() const noexcept { return begin_time_; }
		void set_begin_time( comment_time value ) noexcept { begin_time_ = std::move( value ); }
		comment_time end_time() const noexcept { return end_time_; }
		void set_end_time( comment_time value ) noexcept { end_time_ = std::move( value ); }

		comment_position top() const noexcept { return top_; }
		void set_top( comment_position value ) noexcept { top_ = std::move( value ); }
		comment_position left() const noexcept { return left_; }
		void set_left( comment_position value ) noexcept { left_ = std::move( value ); }
		comment_position height() const noexcept { return height_; }
		void set_height( comment_position value ) noexcept { height_ = std::move( value ); }
		comment_position width() const noexcept { return width_; }
		void set_width( comment_position value ) noexcept { width_ = std::move( value ); }

		comment_analysis_data analysis_data() const noexcept { return analysis_data_; }
		void set_analysis_data( comment_analysis_data value ) noexcept { analysis_data_ = std::move( value ); }

		template<typename T>
		T& extra_data() noexcept { return *reinterpret_cast<T*>( extra_data_ ); }

	private:
#if _WINRT_DLL
		::std::unique_ptr<comment_wrapper> comment_;
#else
		const comment_base* comment_;
#endif
		bool paused_;
		comment_position font_size_;
		comment_alpha alpha_;
		comment_time begin_time_, end_time_;
		comment_position top_, left_, height_, width_;
		comment_analysis_data analysis_data_;
		void* extra_data_;
	};

} }