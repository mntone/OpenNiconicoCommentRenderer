#pragma once

namespace Mntone { namespace Nico { namespace Renderer { namespace DirectX {

	class StepTimer final
	{
		static const uint64 TICKS_PER_SECOND = 10000000;

	public:
		StepTimer();

		void Reset();
		void Update( ::std::function<void()> updateFunc );

	public:
		uint64 PreviousTime() const { return previousTime_; }
		uint64 TimeCounter() const { return timeCounter_; }
		uint64 TotalTimeCounter() const { return totalTimeCounter_; }
		uint64 MaxTimeDelta() const { return maxTimeDelta_; }

		uint64 ElapsedTicks() const { return elapsedTicks_; }
		uint64 TotalTicks() const { return totalTicks_; }

		uint64 Frequency() const { return frequency_; }

		uint16 FramesInThisSecond() const { return framesInThisSecond_; }
		uint16 FramesPerSecond() const { return framesPerSecond_; }
		uint64 TotalFrames() const { return totalFrames_; }

	private:
		uint64 previousTime_, timeCounter_, totalTimeCounter_;
		mutable uint64 maxTimeDelta_;
		uint64 elapsedTicks_, totalTicks_;
		mutable uint64 frequency_;
		uint16 framesInThisSecond_, framesPerSecond_;
		uint64 totalFrames_;
	};

} } } }