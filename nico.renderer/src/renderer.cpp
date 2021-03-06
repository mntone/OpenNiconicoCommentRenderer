#include "pch.h"
#include "renderer.h"
#include "comment_analyzer.h"

using namespace std;
using namespace nico::renderer;

renderer::renderer( renderer_driver& driver )
	: driver_( driver )
	, scale_( 1.f )
{
	comment_analyzer::initialize();

	for( auto& comment : comments_ )
	{
		comment.extra_data_ = driver_.initialize_extra_data();
	}
}

void renderer::reset() noexcept
{
	resources_manager::reset();
}

#if _WINRT_DLL
void renderer::add( ::Mntone::Nico::Renderer::IComment^ comment ) noexcept
#else
void renderer::add( const comment_base& comment ) noexcept
#endif
{
	lock_guard<mutex> lock( *mutex_ );

	auto& rc = resources_manager::get( comment );
	driver_.release_extra_data( rc.extra_data_ );
	rc.extra_data_ = driver_.initialize_extra_data();
	comment_analyzer::analysis( rc );
	set_time( rc );
	set_size( rc );
	set_position( rc );
	resources_manager::add( rc );
}

void renderer::prepare() noexcept
{
	const auto& now = driver_.now();

	{
		lock_guard<mutex> lock( *mutex_ );
		for( auto&& comment : availble_comments_ )
		{
			comment->set_left( calculate_left_position( now, *comment ) );
		}
		auto itr = availble_comments_.cbegin();
		while( itr != availble_comments_.cend() )
		{
			itr = ::std::find_if( itr, availble_comments_.cend(), [=]( rendering_comment* const comment )
			{
				return !comment->paused() && now > comment->end_time();
			} );
			if( itr != availble_comments_.end() )
			{
				driver_.release_extra_data( ( *itr )->extra_data_ );
				( *itr )->extra_data_ = driver_.initialize_extra_data();
				itr = resources_manager::remove( itr );
			}
		}
	}
}

void renderer::render() const noexcept
{
	lock_guard<mutex> lock( *mutex_ );
	driver_.render( availble_comments_ );
}

void renderer::set_time( rendering_comment& comment ) noexcept
{
	const auto& now = driver_.now();
	comment.set_begin_time( now );
	comment.set_end_time( comment.is_not_center() ? now + top_or_bottom_show_time : now + default_show_time );
}

void renderer::set_size( rendering_comment& comment ) noexcept
{
	comment.set_font_size( static_cast<comment_position>( max( minimum_font_size, scale_ * static_cast<float>( comment.font_size() ) ) ) );

	comment_text_info text_info = driver_.text_info( comment );
	if( text_info.height > one_third_of_height() )
	{
		driver_.release_extra_data( comment.extra_data_ );
		comment.extra_data_ = driver_.initialize_extra_data();
		comment.set_font_size( 0.5f * comment.font_size() );
		text_info = driver_.text_info( comment );
	}
	if( comment.is_not_center() && text_info.width > width() )
	{
		driver_.release_extra_data( comment.extra_data_ );
		comment.extra_data_ = driver_.initialize_extra_data();
		comment.set_font_size( comment.font_size() * width() / text_info.width );
		text_info = driver_.text_info( comment );
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
		comment.set_top( calculate_top_position_in_overlap_mode( comment ) );
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

comment_position renderer::calculate_top_position_in_overlap_mode( const rendering_comment& comment ) noexcept
{
	//if( is_floating_point<comment_position>::value )
	//{
	if( mode() == comment_mode_type::both )
	{
		return uniform_int_distribution<uint32_t>( 0, 1 )( mt_ )
			? uniform_real_distribution<comment_position>( top(), bottom_in_top_mode() - comment.height() )( mt_ )
			: uniform_real_distribution<comment_position>( top_in_bottom_mode(), bottom() - comment.height() )( mt_ );
	}
	return uniform_real_distribution<comment_position>( top(), bottom() - comment.height() )( mt_ );
	//}
	//if( mode() == comment_mode_type::both )
	//{
	//	return uniform_int_distribution<uint32_t>( 0, 1 )( mt_ )
	//		? uniform_int_distribution<comment_position>( top(), bottom_in_top_mode() )( mt_ )
	//		: uniform_int_distribution<comment_position>( top_in_bottom_mode(), bottom() )( mt_ );
	//}
	//return uniform_int_distribution<comment_position>( top(), bottom() )( mt_ );
}

comment_position renderer::calculate_left_position( const comment_time time, const rendering_comment& comment ) noexcept
{
	return comment.is_not_center()
	? ( width() - comment.width() ) / 2.f
		: width() - static_cast<comment_position>( ( time - comment.begin_time() ).count() ) / static_cast<comment_position>( default_show_time.count() ) * ( width() + comment.width() );
}

comment_position renderer::move_up( const rendering_comment& self, const rendering_comment& other )
{
	comment_position ret = other.top() - self.height() - 1;
	if( mode() == comment_mode_type::both && ret < top_in_bottom_mode() )
	{
		ret = std::min( ret, bottom_in_top_mode() - self.height() );
	}
	return ret;
}

comment_position renderer::move_down( const rendering_comment& other )
{
	comment_position ret = other.bottom() + 1;
	if( mode() == comment_mode_type::both && ret > bottom_in_top_mode() - other.height() )
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