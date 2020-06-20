
#include <chrono>
#include <memory>

#include "loop.h"
#include <iostream>
namespace CNGE {

	///
	/// will keep loopin and loopin until exit condition
	/// is satisfied,
	/// takes over from anything else on the main thread,
	///
	/// if you try and call this twice then all hell will
	/// rain down upon you
	///
	Loop::Loop(std::function<bool()> exit_condition, std::function<void(Timing*)> frame, u32 maxFps, u32 targetFps) {
		// for vsync, no wait for time
		bool sanic = (targetFps == -1);

		// how long a frame lasts in nanoseconds
		auto frame_time = BILLION / targetFps;

		// set a default value for last time to startLoading
		auto last = std::chrono::high_resolution_clock::now();

		// create frame history, using a maxiumum framerate
		// of which you set using nominal frames
		auto frame_history = new u64[maxFps];
		auto history_start = frame_history;
		auto frame_ptr = history_start;

		// the loop exits if exit condition returns true
		while (!exit_condition())
		{
			// what is the time now
			auto now = std::chrono::high_resolution_clock::now();

			// compare it to the last time
			// to get delta time
			auto delta = u64( (now - last).count() );

			// if a frame should be rendered
			if (sanic || delta >= frame_time)
			{
				// inject delta into history
				*frame_ptr = delta;

				// count the number of fps we have
				auto temp_ptr = frame_ptr;
				auto total_time = 0_u64;
				auto frame_tally = 0_u32;

				// this loop will terminate once the full amount of frame have been recorded
				// or the total time exceeds 1 second
				for (; frame_tally < maxFps; ++frame_tally)
				{
					// if the time we've collected so far
					// goes over a second, then we've
					// found how many frames
					total_time += *temp_ptr;

					if (total_time > BILLION)
						break;

					// go back in time
					--temp_ptr;
					// undeflow and go to end of frame history
					if (temp_ptr < history_start)
						temp_ptr = history_start + maxFps - 1;
				}

				// do frame and pass in timing info
				// calculate the delta time
				auto timing = Timing{ frame_tally, delta, (double)delta / BILLION };
				frame(&timing);

				// set the baseline for the next frame to be right now
				last = now;

				// increment and wrap frame pointer
				++frame_ptr;
				// overflow frame ptr back to the beginning
				if (frame_ptr >= history_start + maxFps - 16)
					frame_ptr = history_start;
			}
		}

		delete[] frame_history;
	}

}
