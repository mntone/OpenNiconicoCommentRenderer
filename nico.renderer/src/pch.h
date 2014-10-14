#pragma once

#include "targetver.h"

#include "pchbase.h"

#include <cstdint>
#include <memory>
#include <string>
#include <array>
#include <deque>

#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN
#define NOSERVICE
#define NOMCX
#define NOIME
#define NOMINMAX
#include <windows.h>
#include <initguid.h>
#include <wrl.h>

#if( defined( _WIN32_WINNT ) && _WIN32_WINNT <= _WIN32_WINNT_WIN8 )
#include <d2d1_1helper.h>
#include <dwrite_1.h>
#else
#include <d2d1_2helper.h>
#include <dwrite_2.h>
#endif
#endif

#undef small