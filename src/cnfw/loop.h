
#ifndef CNFW_LOOP
#define CNFW_LOOP

#include <functional>
#include <vector>
#include <chrono>

#include "types.h"
#include "timing.h"

namespace CNFW {
	class Loop {
	private:
		using nanos = std::chrono::nanoseconds;
		using millis = std::chrono::milliseconds;
		using seconds = std::chrono::seconds;
		using clock = std::chrono::steady_clock;
		using point = std::chrono::time_point<clock>;

		Loop(bool, u32);

		bool sanic;
		u32 fps;

		point last;
		point next;

		nanos frameTime;

		std::vector<nanos> frameHistory;
		std::vector<nanos>::iterator frameIter;

		static auto sleep(i64) -> bool;
		
		auto doFrame(std::function<void(Timing*)>&, point) -> void;
		
	public:
		Loop(u32);
		Loop();

		i32 errCode;
		
		auto begin(std::function<bool()>&&, std::function<void(Timing*)>&&) -> void;

		auto setFPS(u32) -> void;
	};
}

#endif
