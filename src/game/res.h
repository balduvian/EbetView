
#ifndef GAME_GAME_RESOURCES
#define GAME_GAME_RESOURCES

#include "../cnge8/engine/texture/texture.h"
#include "../cnge8/load/bundle/resourceBundle.h"
#include "engine/texture/tileGrid.h"

#include "graphics/rect.h"
#include "graphics/textureShader.h"

namespace Game {
	class Res {
	public:
		static Rect rect;

		static TextureShader textureShader;

		static CNGE::ResourceBundle viewResources;
	};
}

#endif
