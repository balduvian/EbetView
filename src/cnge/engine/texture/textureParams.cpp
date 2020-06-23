
#include <gl/glew.h>

#include "textureParams.h"

namespace CNGE {
	i32 TextureParams::defaultHorzWrap = GL_CLAMP_TO_EDGE;
	i32 TextureParams::defaultVertWrap = GL_CLAMP_TO_EDGE;
	i32 TextureParams::defaultMinFilter = GL_LINEAR;
	i32 TextureParams::defaultMagFilter = GL_NEAREST;

	i32 TextureParams::horzWrap = defaultHorzWrap;
	i32 TextureParams::vertWrap = defaultVertWrap;
	i32 TextureParams::minFilter = defaultMinFilter;
	i32 TextureParams::magFilter = defaultMagFilter;

	TextureParams::TextureParams() {
		TextureParams::horzWrap = defaultHorzWrap;
		TextureParams::vertWrap = defaultVertWrap;
		TextureParams::minFilter = defaultMinFilter;
		TextureParams::magFilter = defaultMagFilter;
	}

	auto TextureParams::setDefaultHorzWrap(i32 horzWrap) -> TextureParams {
		TextureParams::defaultHorzWrap = horzWrap;
		return *this;
	}

	auto TextureParams::setDefaultVertWrap(i32 vertWrap) -> TextureParams {
		TextureParams::defaultVertWrap = vertWrap;
		return *this;
	}

	auto TextureParams::setDefaultMinFilter(i32 minFilter) -> TextureParams {
		TextureParams::defaultMinFilter = minFilter;
		return *this;
	}

	auto TextureParams::setDefaultMagFilter(i32 magFilter) -> TextureParams {
		TextureParams::defaultMagFilter = magFilter;
		return *this;
	}

	auto TextureParams::setHorzWrap(i32 horzWrap) -> TextureParams {
		TextureParams::horzWrap = horzWrap;
		return *this;
	}

	auto TextureParams::setVertWrap(i32 vertWrap) -> TextureParams {
		TextureParams::vertWrap = vertWrap;
		return *this;
	}

	auto TextureParams::setMinFilter(i32 minFilter) -> TextureParams {
		TextureParams::minFilter = minFilter;
		return *this;
	}

	auto TextureParams::setMagFilter(i32 magFilter) -> TextureParams {
		TextureParams::magFilter = magFilter;
		return *this;
	}
}
