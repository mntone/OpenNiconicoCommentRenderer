#include "pch.h"
#include "renderer_window.h"
#include "comment_window.h"
#include "virtual_comment_server.h"

int APIENTRY wWinMain( HINSTANCE hinstance, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*cmd_show*/ )
{
	if( FAILED( CoInitialize( NULL ) ) )
	{
		return 1;
	}

	{
		const auto& cmtsvr = std::make_shared<virtual_comment_server>();

		renderer_window rndwnd( cmtsvr );
		if( SUCCEEDED( rndwnd.initialize( hinstance ) ) )
		{
			std::async( std::launch::async, [&cmtsvr, hinstance, &rndwnd]
			{
				comment_window cmdwnd( cmtsvr );
				if( SUCCEEDED( cmdwnd.initialize( hinstance, rndwnd ) ) )
				{
					cmdwnd.run();
				}
			} );

			rndwnd.run();
		}
	}
	CoUninitialize();

	return 0;
}