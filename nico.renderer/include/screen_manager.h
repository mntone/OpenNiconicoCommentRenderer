#pragma once
#include "comment_base.h"

namespace nico { namespace renderer {

	class screen_manager
	{
	public:
		screen_manager()
			: is_header_view_( false )
			, my_top_( top_ )
			, my_bottom_( bottom_ )
			, mode_( comment_mode::default )
		{ }
		screen_manager( const screen_manager& ) = delete;
		screen_manager( screen_manager&& ) = delete;

		screen_manager& operator=( const screen_manager& ) = delete;
		screen_manager& operator=( screen_manager&& ) = delete;

	public:
		bool is_header_view() const noexcept { return is_header_view_; }
		void set_is_header_view( bool value ) noexcept
		{
			is_header_view_ = std::move( value );
			set_size();
		}

		comment_mode mode() const noexcept { return mode_; }
		void set_mode( comment_mode value ) noexcept
		{
			mode_ = std::move( value );
			set_size();
		}

	protected:
		comment_position height() const noexcept { return bottom() - top(); }
		comment_position width() const noexcept { return right() - left(); }

	private:
		void set_size()
		{
			my_top_ = mode_ == comment_mode::bottom ? top_in_bottom_mode_ : mode_ == comment_mode::top ? 0 : is_header_view_ ? top_with_header_ : top_;
			my_bottom_ = mode_ == comment_mode::top ? bottom_in_top_mode_ : bottom_;
		}

	protected:
		comment_position top() const noexcept { return my_top_; }
		comment_position left() const noexcept { return left_; }
		comment_position bottom() const noexcept { return my_bottom_; }
		comment_position right() const noexcept { return right_; }

		comment_position bottom_in_top_mode() const noexcept { return bottom_in_top_mode_; }
		comment_position top_in_bottom_mode() const noexcept { return top_in_bottom_mode_; }

	private:
		const comment_position top_ = 0;
		const comment_position top_with_header_ = 56;
		const comment_position left_ = 0;
		const comment_position bottom_ = 384;
		const comment_position right_ = 672;
		const comment_position top_in_bottom_mode_ = static_cast<comment_position>( std::is_floating_point<comment_position>::value ? 0.8 * bottom_ : std::ceil( 0.8 * bottom_ ) );
		const comment_position bottom_in_top_mode_ = static_cast<comment_position>( std::is_floating_point<comment_position>::value ? 0.2 * bottom_ : std::floor( 0.2 * bottom_ ) );

		bool is_header_view_;
		comment_position my_top_, my_bottom_;
		comment_mode mode_;
	};

} }