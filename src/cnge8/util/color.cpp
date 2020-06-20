
#include "color.h"

namespace CNGE {
	Color::Color(u32 rgb) {
		/* no alpha */
		if (rgb <= 0xffffff) {
			r = ((rgb >> 16)       ) / 0xff_f32;
			g = ((rgb >>  8) & 0xff) / 0xff_f32;
			b = ((rgb      ) & 0xff) / 0xff_f32;
			a = DEFAULT_ALPHA;

		/* with alpha */
		} else {
			r = ((rgb >> 24)       ) / 0xff_f32;
			g = ((rgb >> 16) & 0xff) / 0xff_f32;
			b = ((rgb >>  8) & 0xff) / 0xff_f32;
			a = ((rgb      ) & 0xff) / 0xff_f32;
		}
	}

	Color::Color(i32 r, i32 g, i32 b) {
		this->r = r / 0xff_f32;
		this->g = g / 0xff_f32;
		this->b = b / 0xff_f32;
		this->a = DEFAULT_ALPHA;
	}

	Color::Color(i32 r, i32 g, i32 b, i32 a) {
		this->r = r / 0xff_f32;
		this->g = g / 0xff_f32;
		this->b = b / 0xff_f32;
		this->a = a / 0xff_f32;
	}

	Color::Color(f32 r, f32 g, f32 b, f32 a)
		: r(r), g(g), b(b), a(a) {}

	Color Color::invert() const {
		return Color(1 - r, 1 - g, 1 - b, a);
	}
}
