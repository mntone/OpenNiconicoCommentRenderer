#pragma once

#define NOMINMAX

#include <mutex>

#include <initguid.h>
#include <collection.h>
#include <ppltasks.h>
#include <wrl.h>

#include <d3d11_2.h>
#include <d2d1_2.h>
#include <d2d1_2helper.h>
#include <dxgi1_2.h>
#include <dwrite_2.h>

#include <windows.ui.xaml.media.dxinterop.h>

#define NICORIMP
#define noexcept throw()
#undef small