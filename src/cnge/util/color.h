
#ifndef CNGE_COLOR
#define CNGE_COLOR

#include "types.h"

namespace CNGE {
	class Color {
	public:
		constexpr static f32 DEFAULT_ALPHA = 1.0_f32;

		f32 r;
		f32 g;
		f32 b;
		f32 a;
		
		Color(u32);
		Color(i32, i32, i32);
		Color(i32, i32, i32, i32);

		Color(f32, f32, f32, f32);

		Color invert() const;
	};
}

#endif
