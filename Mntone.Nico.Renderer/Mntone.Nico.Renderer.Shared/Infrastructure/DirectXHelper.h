#pragma once

namespace Mntone { namespace Nico { namespace Renderer { namespace DirectX {

	ref class DirectXHelper sealed
	{
	internal:
		static void CheckHResult( HRESULT hr );
	};

} } } }