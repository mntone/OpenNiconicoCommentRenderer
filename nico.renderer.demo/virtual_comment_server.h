#pragma once
#include <functional>
#include "comment_sample.h"

class virtual_comment_server
{
public:
	void add_handler(
		std::function<void( const std::shared_ptr<comment_sample> )> comment_handler,
		std::function<void( const nico::renderer::comment_mode_type )> comment_mode_handler,
		std::function<void()> reset_handler ) noexcept;

	void post( const std::shared_ptr<comment_sample> comment ) const noexcept;
	void change_mode( const nico::renderer::comment_mode_type mode ) const noexcept;
	void autotest() const noexcept;
	void reset() const noexcept;

private:
	std::function<void( const std::shared_ptr<comment_sample> )> comment_handler_;
	std::function<void( const nico::renderer::comment_mode_type )> comment_mode_handler_;
	std::function<void()> reset_handler_;
};