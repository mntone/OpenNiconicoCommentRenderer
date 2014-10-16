#pragma once

namespace Mntone { namespace DirectX {

	ref class DirectXHelper sealed
	{
	internal:
		static void CheckHResult( HRESULT hr );
	};

} }