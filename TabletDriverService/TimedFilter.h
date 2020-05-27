#pragma once
#include <atomic>
class TimedFilter
{
public:

	std::atomic_bool cancellation_token = true;
	void FilterTimerCallback(int timeinterval);
};

