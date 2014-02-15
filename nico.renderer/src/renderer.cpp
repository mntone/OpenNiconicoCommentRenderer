#include "pch.h"
#include "renderer.h"

using namespace nico::renderer;

void renderer::add( const comment_base& comment ) noexcept
{
	std::lock_guard<std::mutex> lock( *mutex_ );

	auto& rc = resources_manager::get( comment );
	set_time( rc );
	set_width( rc );
	set_position( rc );
	resources_manager::add( rc );
}

void renderer::render() noexcept
{
	const auto& now = driver_.now();

	{
		std::lock_guard<std::mutex> lock( *mutex_ );
		const auto& itr = std::remove_if( availble_comments_.begin(), availble_comments_.end(), [&]( rendering_comment* const comment )
		{
			if( !comment->paused() && now > comment->end_time() )
			{
				return true;
			}
			comment->set_left( calculate_left_position( now, *comment ) );
			return false;
		} );

		remove( itr, availble_comments_.end() );

		driver_.render( availble_comments_ );
	}
}

void renderer::set_time( rendering_comment& comment ) noexcept
{
	const auto& now = driver_.now();
	comment.set_begin_time( now );
	comment.set_end_time( comment.is_not_center() ? now + top_or_bottom_show_time : now + default_show_time );
}

void renderer::set_width( rendering_comment& comment ) noexcept
{
	comment_text_info text_info = driver_.text_info( comment.font_size(), comment.value() );
	if( comment.is_not_center() && text_info.width > width() )
	{
		comment.set_font_size( comment.font_size() * width() / text_info.width );
		text_info = driver_.text_info( comment.font_size(), comment.value() );
	}
	comment.set_height( text_info.height );
	comment.set_width( text_info.width );
}

void renderer::set_position( rendering_comment& comment ) noexcept
{
	bool is_overlap = false, is_loop = false;
	const auto& now = driver_.now();
	comment.set_top( calculate_top_position( comment ) );
	comment.set_left( calculate_left_position( now, comment ) );
	do
	{
		is_loop = false;
		for( const auto& o : availble_comments_ )
		{
			const auto& other = *o;
			if( comment.is_not_center() == other.is_not_center() && hittest_vertically( comment, other ) )
			{
				const auto& begin_time = std::max( comment.begin_time(), other.begin_time() );
				const auto& end_time = std::min( comment.end_time(), other.end_time() );

				if( other.paused()
					|| hittest_horizontally( begin_time, comment, other )
					|| hittest_horizontally( end_time, comment, other ) )
				{
					if( comment.is_bottom() )
					{
						comment.set_top( move_up( comment, other ) );
						is_overlap = check_overlap_for_bottom( comment );
					}
					else
					{
						comment.set_top( move_down( other ) );
						is_overlap = check_overlap( comment );
					}
					if( is_overlap )
					{
						break;
					}
					is_loop = true;
					break;
				}
			}
		}
	} while( is_loop );
	if( is_overlap )
	{
		comment.set_top( calculate_top_position_in_overlap_mode() );
		comment.set_alpha( overlap_comment_alpha );
	}
}

bool renderer::hittest_vertically( const rendering_comment& a, const rendering_comment& b )
{
	return hittest( a.top(), a.bottom(), b.top(), b.bottom() );
}

bool renderer::hittest_horizontally( const comment_time time, const rendering_comment& a, const rendering_comment& b )
{
	const auto& a_left = calculate_left_position( time, a );
	const auto& b_left = calculate_left_position( time, b );
	return hittest( a_left, a_left + a.width(), b_left, b_left + b.width() );
}

bool renderer::hittest( comment_position a1, comment_position a2, comment_position b1, comment_position b2 )
{
	return a1 < b2 && a2 > b1;
}

comment_position renderer::calculate_top_position( const rendering_comment& comment ) noexcept
{
	return comment.is_bottom() ? bottom() - comment.height() : top();
}

comment_position renderer::calculate_top_position_in_overlap_mode() noexcept
{
	//if( std::is_floating_point<comment_position>::value )
	//{
		if( mode() == comment_mode::both )
		{
			return std::uniform_int_distribution<uint32_t>( 0, 1 )( mt_ )
				? std::uniform_real_distribution<comment_position>( top(), bottom_in_top_mode() )( mt_ )
				: std::uniform_real_distribution<comment_position>( top_in_bottom_mode(), bottom() )( mt_ );
		}
		return std::uniform_real_distribution<comment_position>( top(), bottom() )( mt_ );
	//}
	//if( mode() == comment_mode::both )
	//{
	//	return std::uniform_int_distribution<uint32_t>( 0, 1 )( mt_ )
	//		? std::uniform_int_distribution<comment_position>( top(), bottom_in_top_mode() )( mt_ )
	//		: std::uniform_int_distribution<comment_position>( top_in_bottom_mode(), bottom() )( mt_ );
	//}
	//return std::uniform_int_distribution<comment_position>( top(), bottom() )( mt_ );
}

comment_position renderer::calculate_left_position( const comment_time time, const rendering_comment& comment ) noexcept
{
	return comment.is_not_center()
		? ( width() - comment.width() ) / 2.0f
		: width() - static_cast<comment_position>( time - comment.begin_time() ) / static_cast<comment_position>( default_show_time ) * ( width() + comment.width() );
}

comment_position renderer::move_up( const rendering_comment& self, const rendering_comment& other )
{
	comment_position ret = other.top() - self.height() - 1;
	if( mode() == comment_mode::both && ret < top_in_bottom_mode() )
	{
		ret = std::min( ret, bottom_in_top_mode() - self.height() );
	}
	return ret;
}

comment_position renderer::move_down( const rendering_comment& other )
{
	comment_position ret = other.bottom() + 1;
	if( mode() == comment_mode::both && ret > bottom_in_top_mode() - other.height() )
	{
		ret = std::max( ret, top_in_bottom_mode() );
	}
	return ret;
}

bool renderer::check_overlap( const rendering_comment& comment )
{
	return comment.bottom() > bottom() ? true : false;
}

bool renderer::check_overlap_for_bottom( const rendering_comment& comment )
{
	return comment.top() < top() ? true : false;
}