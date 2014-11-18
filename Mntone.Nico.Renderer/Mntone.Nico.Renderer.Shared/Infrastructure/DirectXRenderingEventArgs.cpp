#include "pch.h"
#include "DirectXRenderingEventArgs.h"

using namespace Mntone::Nico::Renderer::DirectX;

DirectXRenderingEventArgs::DirectXRenderingEventArgs( uint64 totalFrames )
	: FramesPerSecond_( 0 )
	, TotalFrames_( totalFrames )
{ }

DirectXRenderingEventArgs::DirectXRenderingEventArgs( uint16 framesPerSecond, uint64 totalFrames )
	: FramesPerSecond_( framesPerSecond )
	, TotalFrames_( totalFrames )
{ }