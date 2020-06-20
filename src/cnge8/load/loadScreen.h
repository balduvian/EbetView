
#ifndef CNGE_LOAD_SCREEN
#define CNGE_LOAD_SCREEN

#include "../engine/input.h"
#include "../engine/loop.h"
#include "resource.h"

namespace CNGE {
	class LoadScreen {
	public:
		LoadScreen();

		virtual void resized(i32, i32) = 0;

		virtual void update(Input*, Timing*) = 0;

		virtual void render(Resource*, i32, i32) = 0;
	};
}

#endif
