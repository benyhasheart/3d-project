#include "YonTimer.h"

using namespace std::chrono;

YonTimer::YonTimer() noexcept
{
	mLastTime = steady_clock::now();
	mCurrentTime = steady_clock::now();
	mFrameTime = mLastTime - mCurrentTime;
}

float YonTimer::Mark() noexcept
{
	const auto oldTime = mLastTime;
	mLastTime = steady_clock::now();
	mFrameTime = mLastTime - oldTime;
	
	return mFrameTime.count();
}
