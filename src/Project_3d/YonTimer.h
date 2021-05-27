#pragma once
#include <chrono>

class YonTimer final
{
public:
	YonTimer() noexcept;
	~YonTimer() = default;
public:
	float Mark() noexcept;
private:
	std::chrono::steady_clock::time_point mLastTime;
	std::chrono::steady_clock::time_point mCurrentTime;
	std::chrono::duration<float> mFrameTime;
};

