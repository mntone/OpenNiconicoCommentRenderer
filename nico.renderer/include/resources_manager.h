#pragma once
#include <array>
#include <deque>
#include <mutex>
#include <memory>
#include "comment_base.h"

namespace nico { namespace renderer {

	class resources_manager
	{
	public:
		resources_manager()
			: mutex_( std::make_shared<std::mutex>() )
		{
			for( auto& comment : comments_ )
			{
				comments_pool_.emplace_back( &comment );
			}
		}
		resources_manager( const resources_manager& ) = delete;
		resources_manager( resources_manager&& ) = delete;

		resources_manager& operator=( const resources_manager& ) = delete;
		resources_manager& operator=( resources_manager&& ) = delete;

	protected:
		rendering_comment& get( const comment_base& comment )
		{
			rendering_comment* buf;
			if( comments_pool_.empty() )
			{
				buf = availble_comments_.front();
				availble_comments_.pop_front();
			}
			else
			{
				buf = comments_pool_.front();
				comments_pool_.pop_front();
			}
			buf->set( comment );
			return *buf;
		}

		void add( rendering_comment& comment )
		{
			availble_comments_.push_back( &comment );
		}

	protected:
		mutable std::shared_ptr<std::mutex> mutex_;
		std::array<rendering_comment, maximum_comment_visible_count> comments_;
		std::deque<rendering_comment*> availble_comments_, comments_pool_;
	};

} }