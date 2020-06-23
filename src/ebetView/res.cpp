
#include "res.h"

namespace Game {
	Rect Res::rect = Rect();

	TextureShader Res::textureShader = TextureShader();

	CNGE::ResourceBundle Res::viewResources = CNGE::ResourceBundle({
		&rect, &textureShader
	});
}
