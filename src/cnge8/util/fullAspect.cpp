
#include "fullAspect.h"

namespace CNGE {

	FullAspect::FullAspect() : width(0), height(0) {}
	
	auto FullAspect::update(u32 screenWidth, u32 screenHeight) -> void {
		width = screenWidth;
		height = screenHeight;
	}
	
	auto FullAspect::getLeft() const -> u32 {
		return 0;
	}
	
	auto FullAspect::getTop() const -> u32 {
		return 0;
	}
	
	auto FullAspect::getWidth() const -> u32 {
		return width;
	}
	
	auto FullAspect::getHeight() const -> u32 {
		return height;
	}
}
