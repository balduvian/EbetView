
#ifndef CNFW_TIMING
#define CNFW_TIMING

#include "types.h"

namespace CNFW {
	class Timing {
	public:
		constexpr static u64 BILLION = 1000000000;

		u32 fps;
		u64 delta;
		f64 time;

		Timing(u32, u64, f64);

		Timing(const Timing&) = default;
		auto operator=(const Timing&) -> Timing& = default;
	};
}

#endif
