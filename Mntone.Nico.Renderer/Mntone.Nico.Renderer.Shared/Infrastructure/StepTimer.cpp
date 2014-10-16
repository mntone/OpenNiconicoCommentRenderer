#include "pch.h"
#include "StepTimer.h"

using namespace Platform;
using namespace std;
using namespace Mntone::DirectX;

StepTimer::StepTimer()
{
	if( !QueryPerformanceFrequency( reinterpret_cast<LARGE_INTEGER*>( &frequency_ ) ) )
	{
		throw ref new FailureException();
	}
	maxTimeDelta_ = frequency_ / 10;

	Reset();
}

void StepTimer::Reset()
{
	if( !QueryPerformanceCounter( reinterpret_cast<LARGE_INTEGER*>( &previousTime_ ) ) )
	{
		throw ref new FailureException();
	}
	timeCounter_ = 0;
	elapsedTicks_ = 0;
	totalTicks_ = 0;
	framesInThisSecond_ = 0;
	framesPerSecond_ = 0;
	totalFrames_ = 0;
}

void StepTimer::Update( function<void()> updateFunc )
{
	uint64 currentTime = 0;
	if( !QueryPerformanceCounter( reinterpret_cast<LARGE_INTEGER*>( &currentTime ) ) )
	{
		throw ref new FailureException();
	}

	auto timeDelta = currentTime - previousTime_;
	previousTime_ = currentTime;
	timeCounter_ += timeDelta;
	totalTimeCounter_ += timeDelta;
	
	auto tickDelta = TICKS_PER_SECOND * min( timeDelta, maxTimeDelta_ ) / frequency_;
	auto previousFrameCount = totalFrames_;
	elapsedTicks_ = tickDelta;
	totalTicks_ += tickDelta;
	++framesInThisSecond_;
	++totalFrames_;
	updateFunc();

	if( timeCounter_ >= frequency_ )
	{
		framesPerSecond_ = framesInThisSecond_;
		framesInThisSecond_ = 0;
		timeCounter_ %= frequency_;
	}
}