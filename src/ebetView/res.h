
#ifndef EBETVIEW_GAME_RESOURCES
#define EBETVIEW_GAME_RESOURCES

#include "cnge/engine/texture/texture.h"
#include "cnge/engine/texture/tileGrid.h"
#include "cnge/load/bundle/resourceBundle.h"

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
