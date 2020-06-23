
#include <memory>
#include <iostream>
#include <thread>
#include <string>

#include "loop.h"

namespace CNFW {
	Loop::Loop(bool sanic, u32 fps) : 
		sanic(sanic), fps(fps), frameHistory(fps), frameTime(Timing::BILLION / fps), last(), next(), frameIter(frameHistory.begin()) {}

	auto Loop::doFrame(std::function<void(Timing*)> frame) -> void {
		auto now = clock::now();

		/* calculate how long this frame took */
		auto delta = (now - last);

		/* frame will happen exactly frameTime away from last frame */
		/* unless there is severe lag, then the baseline for a second shifts */
		next = (delta < frameTime * 2) ? last + frameTime : now + frameTime;
		last = now;

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

			if (totalTime > std::chrono::seconds(1))
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
	
	auto Loop::begin(std::function<bool()> exit, std::function<void(Timing*)> frame) -> void {
		/* in nanoseconds */
		auto frameTime = std::chrono::duration_cast<nanos>(seconds(1) / f64(fps));

		last = clock::now();
		next = clock::now();

		while (!exit()) {
			if (!sanic) {
				auto now = clock::now();

				auto timeUntilFrame = next - now;

				std::this_thread::sleep_for(timeUntilFrame);
			}

			doFrame(frame);
		}
	}

	auto Loop::setFPS(u32 fps) -> void {
		this->fps = fps;
	}
}
