
#ifndef CNGE_ASPECT
#define CNGE_ASPECT

#include "../Types.h"

namespace CNGE {
	class Aspect {
	private:
		/// values we use to calculate
		f64 aspWidth, aspHeight;
		f64 maxWidth, maxHeight;

		/// calculated values we want
		f64 gameWidth, gameHeight;
		u32 portX, portY, portW, portH;

		/// helper readability functions
		f64 calcWidth(f64 aspect, f64 height);
		f64 calcHeight(f64 aspect, f64 width);

	public:
		Aspect(f64 width, f64 height, f64 maxWidth, f64 maxHeight);

		Aspect(Aspect&&) = delete;

		void changeAspect(f64 width, f64 height);
		void changeMaxAspect(f64 width, f64 height);

		void update(u32 screenWidth, u32 screenHeight);

		f64 getGameWidth();
		f64 getGameHeight();

		u32 getLeft();
		u32 getTop();
		u32 getWidth();
		u32 getHeight();

		auto screenToGameX(i32) -> f32;
		auto screenToGameY(i32) -> f32;
	};

}

#endif
