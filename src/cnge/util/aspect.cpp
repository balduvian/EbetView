
#include "aspect.h"

namespace CNGE {
	Aspect::Aspect(f64 width, f64 height, f64 maxWidth, f64 maxHeight)
	: gameWidth(), gameHeight(), portX(), portY(), portW(), portH(),
	aspWidth(width), aspHeight(height), maxWidth(maxWidth), maxHeight(maxHeight) {}

	void Aspect::changeAspect(f64 width, f64 height) {
		aspWidth = width;
		aspHeight = height;
	}

	void Aspect::changeMaxAspect(f64 width, f64 height) {
		maxWidth = width;
		maxHeight = height;
	}

	f64 Aspect::calcWidth(f64 aspect, f64 height) {
		return aspect * height;
	}

	f64 Aspect::calcHeight(f64 aspect, f64 width) {
		return (1._f64 / aspect) * width;
	}

	/// call this whenever the screen changes size
	/// or whenever you need to get initial game dimensions
	void Aspect::update(u32 screenWidth, u32 screenHeight) {
		// we need to know the aspect of the window
		auto prefAspect = aspWidth / aspHeight;
		auto screenAspect = (f64)screenWidth / screenHeight;

		// if the window is really tall
		if (prefAspect > screenAspect) {
			// the game is all the way across
			// the width of the window
			gameWidth = aspWidth;
			portX = 0;
			portW = screenWidth;

			// the tallest we want it to be
			auto minAspect = aspWidth / maxHeight;

			// limit our aspect
			if (screenAspect < minAspect)
				screenAspect = minAspect;
			
			// position centered vertically
			portH = calcHeight(screenAspect, portW);
			portY = (screenHeight - portH) / 2;

			// and then finally use it to calculate gameHeight
			gameHeight = calcHeight(screenAspect, gameWidth);
		}
		// if the window is long
		else {
			// the game takes up all the vertical space
			gameHeight = aspHeight;
			portY = 0;
			portH = screenHeight;

			// the widest we want it to be
			auto maxAspect = maxWidth / aspHeight;

			// limit our aspect
			if (screenAspect > maxAspect)
				screenAspect = maxAspect;

			// position centered vhorizontally
			portW = calcWidth(screenAspect, portH);
			portX = (screenWidth - portW) / 2;

			// and then finally use it to calculate gameWidth
			gameWidth = calcWidth(screenAspect, gameHeight);
		}
	}

	/*
	 * getters
	 */

	f64 Aspect::getGameWidth() {
		return gameWidth;
	}

	f64 Aspect::getGameHeight() {
		return gameHeight;
	}

	u32 Aspect::getLeft() {
		return portX;
	}

	u32 Aspect::getTop() {
		return portY;
	}

	u32 Aspect::getWidth() {
		return portW;
	}

	u32 Aspect::getHeight() {
		return portH;
	}

	auto Aspect::screenToGameX(i32 screenX) -> f32 {
		screenX -= portX;
		auto screenT = screenX / (f32)portW;
		return screenT *= gameWidth;
	}

	auto Aspect::screenToGameY(i32 screenY) -> f32 {
		screenY -= portY;
		auto screenT = screenY / (f32)portH;
		return screenT *= gameHeight;
	}
}
