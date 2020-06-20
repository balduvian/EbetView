
#pragma once

#include <functional>

#include "../Types.h"

namespace CNGE {
	struct Timing {
		u32 fps;
		u64 delta;
		f64 time;
	};

	class Loop {
	private:
		constexpr static f64 BILLION = 1000000000;

	public:
		/// will keep loopin and loopin until exit condition is satisfied,
		/// takes over from anything else on the main thread,
		Loop(std::function<bool()> exit_condition, std::function<void(Timing*)> frame, u32 maxFps, u32 targetFps = -1);
	};
}
