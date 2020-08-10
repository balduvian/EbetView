
#include <memory>
#include <iostream>
#include <thread>
#include <string>

#include "window.h"

#include "loop.h"

namespace CNFW {
	Loop::Loop(bool sanic, u32 fps) : 
		sanic(sanic), fps(fps), frameTime(std::chrono::duration_cast<nanos>(seconds(1) / f64(fps))), frameHistory(300), frameIter(frameHistory.begin()), errCode(0) {
	}

	auto Loop::sleep(const i64 ns) -> bool {
		/* Declarations */
		HANDLE timer;   /* Timer handle */
		LARGE_INTEGER li;   /* Time defintion */
		/* Create timer */
		if (!(timer = CreateWaitableTimer(nullptr, true, nullptr)))
			return false;
		/* Set timer properties */
		li.QuadPart = -ns;
		if (!SetWaitableTimer(timer, &li, 0, nullptr, nullptr, false)) {
			CloseHandle(timer);
			return false;
		}
		/* Start & wait for timer */
		WaitForSingleObject(timer, INFINITE);
		/* Clean resources */
		CloseHandle(timer);
		/* Slept without problems */
		return true;
	}
	
	auto Loop::doFrame(std::function<void(Timing*)>& frame, const point now) -> void {
		/* calculate how long this frame took */
		const auto delta = now - last;

		if (delta < frameTime * 2) {
			/* no lag happened, frames as usual */
			next = last + frameTime * 2;
			last = last + frameTime;
		} else {
			/* lag detected! reset */
			next = now + frameTime;
			last = now;
		}

		OutputDebugString((std::to_wstring(delta.count()) + L"\n").c_str());

		/* record how long this frame took */
		*frameIter = delta;

		/* calculate nominal fps */
		auto tempIter = frameIter;
		auto totalTime = nanos(0);
		auto frameTally = 0_u32;

		/* loop goes back in time, starting from the current frame */
		/* counting how many frames have passed in the last second */
		for (; frameTally < fps; ++frameTally) {
			totalTime += *tempIter;

			if (totalTime > seconds(1))
				break;

			/* circle back to end of frame history */
			if (tempIter == frameHistory.begin())
				tempIter = frameHistory.end() - 1;
			else
				--tempIter;
		}

		auto timing = Timing(frameTally, delta.count(), f64(delta.count()) / Timing::BILLION);
		frame(&timing);

		++frameIter;

		/* circle back to start of frame history */
		if (frameIter == frameHistory.end())
			frameIter = frameHistory.begin();
	}

	Loop::Loop(u32 fps) : Loop(false, fps) {}
	Loop::Loop() : Loop(true, 0) {}
	
	auto Loop::begin(std::function<bool()>&& exit, std::function<void(Timing*)>&& frame) -> void {
		last = clock::now();
		next = clock::now();

		while (!exit()) {
			/* sanic mode skips all waiting code */
			if (!sanic) {
				const auto waitTime = std::chrono::duration_cast<millis>((next - clock::now()) - nanos(1000)).count();

				/* let the cpu rest for most of the time */
				/* calculate the time remaining until next frame*/
				sleep(waitTime);
				
				/* spin until it's exactly time for a new frame */
				while (clock::now() < next);
			}

			doFrame(frame, clock::now());
		}
	}

	auto Loop::setFPS(const u32 fps) -> void {
		this->fps = fps;
	}
}
