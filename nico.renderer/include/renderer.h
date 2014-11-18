#pragma once
#include <random>
#include "comment_base.h"
#include "rendering_comment.h"
#include "resources_manager.h"
#include "screen_manager.h"

namespace nico { namespace renderer {

	class renderer final
		: private resources_manager
		, public screen_manager
	{
	public:
		renderer() = delete;
		renderer( const renderer& ) = delete;
		renderer( renderer&& ) = delete;

		renderer& operator=( const renderer& ) = delete;
		renderer& operator=( renderer&& ) = delete;

	public:
		NICORIMP explicit renderer( renderer_driver& driver );

	public:
		NICORIMP void reset() noexcept;
#if _WINRT_DLL
		NICORIMP void add( ::Mntone::Nico::Renderer::IComment^ comment ) noexcept;
#else
		NICORIMP void add( const comment_base& comment ) noexcept;
#endif
		NICORIMP void prepare() noexcept;
		NICORIMP void render() const noexcept;

	private:
		void set_time( rendering_comment& comment ) noexcept;
		void set_size( rendering_comment& comment ) noexcept;
		void set_position( rendering_comment& comment ) noexcept;

		bool hittest_vertically( const rendering_comment& a, const rendering_comment& b );
		bool hittest_horizontally( const comment_time time, const rendering_comment& a, const rendering_comment& b );
		bool hittest( comment_position a1, comment_position a2, comment_position b1, comment_position b2 );
	
		comment_position calculate_top_position( const rendering_comment& comment ) noexcept;
		comment_position calculate_top_position_in_overlap_mode( const rendering_comment& comment ) noexcept;
		comment_position calculate_left_position( const comment_time time, const rendering_comment& comment ) noexcept;

		comment_position move_up( const rendering_comment& self, const rendering_comment& other );
		comment_position move_down( const rendering_comment& other );

		bool check_overlap( const rendering_comment& comment );
		bool check_overlap_for_bottom( const rendering_comment& comment );

	private:
		const comment_time default_show_time = std::chrono::seconds( 5 );
		const comment_time top_or_bottom_show_time = std::chrono::seconds( 3 );

		renderer_driver& driver_;
		std::mt19937 mt_;
	};

} }