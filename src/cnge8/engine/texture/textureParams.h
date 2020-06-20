
#ifndef CNGE_TEXTURE_PARAMS
#define CNGE_TEXTURE_PARAMS

#include "../../types.h"

namespace CNGE {
	class TextureParams {
	private:
		static i32 defaultHorzWrap;
		static i32 defaultVertWrap;
		static i32 defaultMinFilter;
		static i32 defaultMagFilter;

		static i32 horzWrap;
		static i32 vertWrap;
		static i32 minFilter;
		static i32 magFilter;

	public:
		TextureParams();

		auto setDefaultHorzWrap(i32)->TextureParams;
		auto setDefaultVertWrap(i32)->TextureParams;
		auto setDefaultMinFilter(i32)->TextureParams;
		auto setDefaultMagFilter(i32)->TextureParams;

		auto setHorzWrap(i32)->TextureParams;
		auto setVertWrap(i32)->TextureParams;
		auto setMinFilter(i32)->TextureParams;
		auto setMagFilter(i32)->TextureParams;

		friend class Texture;
	};
}

#endif
