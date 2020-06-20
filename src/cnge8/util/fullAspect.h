
#ifndef CNGE_FULL_ASPECT
#define CNGE_FULL_ASPECT

#include "../types.h"

namespace CNGE {
	class FullAspect {
	private:
		/// calculated values we want
		f64 width, height;

	public:
		FullAspect();
		
		~FullAspect() = default;
		FullAspect(const FullAspect&) = default;
		auto operator=(const FullAspect&) -> FullAspect& = default;
		auto operator=(FullAspect&&) -> FullAspect& = default;
		FullAspect(FullAspect&&) = default;
		
		auto update(u32 screenWidth, u32 screenHeight) -> void;

		[[nodiscard]] auto getLeft() const -> u32;
		[[nodiscard]] auto getTop() const -> u32;
		[[nodiscard]] auto getWidth() const -> u32;
		[[nodiscard]] auto getHeight() const -> u32;
	};

}

#endif
