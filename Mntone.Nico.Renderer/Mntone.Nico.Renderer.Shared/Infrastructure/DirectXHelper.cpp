#include "pch.h"
#include "DirectXHelper.h"

using namespace Platform;
using namespace Mntone::DirectX;

void DirectXHelper::CheckHResult( HRESULT hr )
{
	if( FAILED( hr ) )
	{
		throw Exception::CreateException( hr );
	}
}