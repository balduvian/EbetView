
#ifndef GAME_ICON
#define GAME_ICON

#include <windows.h>

#include "types.h"

namespace Game {
	class IconResource {
	private:
		u8* data;
		i32 size;

		static auto getCurrentModule() -> HMODULE;
		
	public:
		explicit IconResource(i32);
		~IconResource();

		[[nodiscard]] auto getData() const -> u8*;
		[[nodiscard]] auto getSize() const -> i32;
	};
}

#endif
