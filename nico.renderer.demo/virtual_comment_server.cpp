#include "pch.h"
#include "virtual_comment_server.h"

using namespace nico::renderer;

void virtual_comment_server::add_handler(
	std::function<void( const std::shared_ptr<comment_sample> )> comment_handler,
	std::function<void( const comment_mode_type )> comment_mode_handler ) noexcept
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

void virtual_comment_server::change_mode( const nico::renderer::comment_mode_type mode ) const noexcept
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
		comment_handler_( std::make_shared<comment_sample>( L"＾＾；", true, false, comment_vertical_position_type::center, comment_size_type::medium, 0xffffff ) );
		comment_handler_( std::make_shared<comment_sample>( L"テスト", true, false, comment_vertical_position_type::center, comment_size_type::medium, 0xffffff ) );
		comment_handler_( std::make_shared<comment_sample>( L"わこつ", true, false, comment_vertical_position_type::center, comment_size_type::medium, 0xffffff ) );
		comment_handler_( std::make_shared<comment_sample>( L"⒮←丸ゴシック (Gulim) 化文字", false, false, comment_vertical_position_type::center, comment_size_type::large, 0xffffff ) );
		comment_handler_( std::make_shared<comment_sample>( L"⑴←明朝 (SimSun) 化文字", false, false, comment_vertical_position_type::center, comment_size_type::large, 0xffffff ) );
		comment_handler_( std::make_shared<comment_sample>( L"ˍ←明朝 (PMingLiU) 化文字", false, false, comment_vertical_position_type::center, comment_size_type::large, 0xffffff ) );
		comment_handler_( std::make_shared<comment_sample>( L"Visual Studio is cool!", false, false, comment_vertical_position_type::bottom, comment_size_type::small, 0xffffff ) );
		comment_handler_( std::make_shared<comment_sample>( L"ながあああああああああああああいおつきあい。京都銀行", false, false, comment_vertical_position_type::center, comment_size_type::medium, 0xffffff ) );
		comment_handler_( std::make_shared<comment_sample>( L"Super Mario BROS. プレイしようぜ!", false, false, comment_vertical_position_type::center, comment_size_type::medium, 0 ) );
		comment_handler_( std::make_shared<comment_sample>( L"てってってー", false, false, comment_vertical_position_type::center, comment_size_type::large, 0x00cc66 ) );
	}
}