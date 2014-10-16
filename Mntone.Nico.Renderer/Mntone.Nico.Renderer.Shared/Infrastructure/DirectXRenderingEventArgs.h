#pragma once

namespace Mntone { namespace DirectX {

	public ref class DirectXRenderingEventArgs sealed
	{
	internal:
		DirectXRenderingEventArgs( uint64 totalFrames );
		DirectXRenderingEventArgs( uint16 framesPerSecond, uint64 totalFrames );

	public:
		property uint16 FramesPerSecond
		{
			uint16 get() { return FramesPerSecond_; }
		}
		property uint64 TotalFrames
		{
			uint64 get() { return TotalFrames_; }
		}

	private:
		uint16 FramesPerSecond_;
		uint64 TotalFrames_;
	};

} }