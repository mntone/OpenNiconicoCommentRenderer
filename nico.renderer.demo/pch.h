#pragma once

#include "targetver.h"

#include <pchbase.h>

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <thread>
#include <future>
#include <cmath>
#include <ppltasks.h>

#define WIN32_LEAN_AND_MEAN
#define NOSERVICE
#define NOMCX
#define NOIME
#define NOMINMAX
#include <windows.h>
#include <initguid.h>
#include <wrl.h>

#if( defined( _WIN32_WINNT ) && _WIN32_WINNT <= _WIN32_WINNT_WIN8 )
#include <d3d11_1.h>
#include <d2d1_1helper.h>
#include <dwrite_1.h>
#else
#include <d3d11_2.h>
#include <d2d1_2helper.h>
#include <dwrite_2.h>
#endif

#define noexcept throw()
#undef small

#include "./mnfx/directx_window.h"

using namespace Microsoft::WRL;