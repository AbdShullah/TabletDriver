#pragma once
#include "TimedFilter.h"
#include <future>
class TabletFilter {
public:
	virtual void SetTarget(Vector2D vector, double h) = 0;
	virtual void SetPosition(Vector2D vector, double h) = 0;
	virtual bool GetPosition(Vector2D* vector) = 0;
	virtual void Update() = 0;

	TimedFilter* timer;
	WAITORTIMERCALLBACK callback;
	double timerInterval;

	std::future<void> task;
	bool isEnabled;
	bool isValid;

	TabletFilter();

	bool StartTimer();
	bool StopTimer();

};

