#include "stdafx.h"
#include "TabletFilter.h"


TabletFilter::TabletFilter() {
	timer = NULL;
	timerInterval = 2;
	isValid = false;
	isEnabled = false;
}

//
// Start Timer
//
bool TabletFilter::StartTimer() {
	timer = new TimedFilter();
	task = async(launch::async, &TimedFilter::FilterTimerCallback, timer, timerInterval);
}


//
// Stop Timer
//
bool TabletFilter::StopTimer() {
	if (timer == NULL) return false;
	timer->cancellation_token = false;
	task.get();
	delete timer;
	timer = NULL;
	return true;
}