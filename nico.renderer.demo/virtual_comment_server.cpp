#include "pch.h"
#include "virtual_comment_server.h"

using namespace nico::renderer;

void virtual_comment_server::add_handler(
	std::function<void( const std::shared_ptr<comment_sample> )> comment_handler,
	std::function<void( const comment_mode )> comment_mode_handler ) noexcept
{
	comment_handler_ = comment_handler;
	comment_mode_handler_ = comment_mode_handler;
}

void virtual_comment_server::post( const std::shared_ptr<comment_sample> comment ) const noexcept
{
	if( comment_handler_ )
	{
		comment_handler_( comment );
	}
}

void virtual_comment_server::change_mode( const nico::renderer::comment_mode mode ) const noexcept
{
	if( comment_mode_handler_ )
	{
		comment_mode_handler_( mode );
	}
}

void virtual_comment_server::autotest() const noexcept
{
	if( comment_handler_ )
	{
		comment_handler_( std::make_shared<comment_sample>( L"���s\n�e�X�g", true, false, comment_vertical_position::center, comment_size::medium, 0xffffff ) );
		comment_handler_( std::make_shared<comment_sample>( L"�킱��", false, false, comment_vertical_position::center, comment_size::medium, 0xffffff ) );
		comment_handler_( std::make_shared<comment_sample>( L"�v��������񂩂킢������������", false, false, comment_vertical_position::center, comment_size::medium, 0xffffff ) );
		comment_handler_( std::make_shared<comment_sample>( L"Visual Studio is cool!", false, false, comment_vertical_position::bottom, comment_size::small, 0xffffff ) );
		comment_handler_( std::make_shared<comment_sample>( L"�Ȃ��������������������������������������B���s��s", false, false, comment_vertical_position::center, comment_size::medium, 0xffffff ) );
		comment_handler_( std::make_shared<comment_sample>( L"Super Mario BROS. �v���C���悤��!", false, false, comment_vertical_position::center, comment_size::medium, 0xffffff ) );
		comment_handler_( std::make_shared<comment_sample>( L"�Ă��Ă��ā[", false, false, comment_vertical_position::center, comment_size::large, 0x00cc66 ) );
	}
}