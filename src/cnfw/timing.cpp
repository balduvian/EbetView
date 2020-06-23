
#include "timing.h"

namespace CNFW {
	Timing::Timing(u32 fps, u64 delta, f64 time) :
		fps(fps), delta(delta), time(time) {}
}
