#include "stdafx.h"
#include "TimedFilter.h"
#include <chrono>
#include <atomic>
#include "Logger.h"
#include "Tablet.h"

chrono::high_resolution_clock::time_point timeBegin2 = chrono::high_resolution_clock::now();



void TimedFilter::FilterTimerCallback(int timeinterval) {

	auto oldTime = chrono::high_resolution_clock::now();

	while (cancellation_token) {
		chrono::high_resolution_clock::time_point timeNow = chrono::high_resolution_clock::now();
		auto deltaTime = std::chrono::duration_cast<chrono::milliseconds>(timeNow - oldTime).count();
		if (deltaTime >= timeinterval) {
			oldTime = timeNow;

			Vector2D position, position_prev;
			double z;
			TabletFilter* filter;




			// Set position
			position.Set(tablet->state.position);
			z = tablet->state.z;
			// For debug
			tablet->filterTimed[0]->GetPosition(&position_prev);

			// Loop through filters
			for (int filterIndex = 0; filterIndex < tablet->filterTimedCount; filterIndex++) {

				// Filter
				filter = tablet->filterTimed[filterIndex];

				// Filter enabled?
				if (!filter->isEnabled) return;

				// Set filter targets
				filter->SetTarget(position, z);
				//filter->z = tablet->state.z;

				// Update filter position
				filter->Update();

				// Set output vector
				filter->GetPosition(&position);

			}


			// Debug messages
			if (tablet->debugEnabled) {
				timeNow = chrono::high_resolution_clock::now();
				double delta = (timeNow - timeBegin2).count() / 1000000.0;

				if (round(position.x * 100) != round(position_prev.x * 100) or round(position.y * 100) != round(position_prev.y * 100)) {
					LOG_DEBUG("FIL:%0.3f,%0.3f,%0.3f\n",
						delta,
						position.x,
						position.y
					);
				}

			}


			//
			// Relative mode
			//
			if (vmulti->mode == VMulti::ModeRelativeMouse) {

				// Map position to virtual screen (values between 0 and 1)
				mapper->GetRotatedTabletPosition(&position.x, &position.y);

				// Create VMulti report
				vmulti->CreateReport(
					tablet->state.buttons,
					position.x,
					position.y,
					tablet->state.pressure
				);

				// Write report to VMulti device if report has changed
				if (vmulti->HasReportChanged()
					||
					vmulti->reportRelativeMouse.x != 0
					||
					vmulti->reportRelativeMouse.y != 0
					) {
					vmulti->WriteReport();
				}


			}

			//
			// Absolute / Digitizer mode
			//
			else {


				// Map position to virtual screen (values betweeb 0->1)
				mapper->GetScreenPosition(&position.x, &position.y);

				// Create VMulti report
				vmulti->CreateReport(
					tablet->state.buttons,
					position.x,
					position.y,
					tablet->state.pressure
				);


				// Write report to VMulti device
				if (vmulti->HasReportChanged() && tablet->state.isValid) {
					vmulti->WriteReport();
				}
			}
		}
		std::this_thread::yield();
	}
}

