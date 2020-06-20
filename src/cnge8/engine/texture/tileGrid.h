
#ifndef CNGE_TILE_GRID
#define CNGE_TILE_GRID

#include "texture.h"

#include "../../types.h"

namespace CNGE {
	class TileGrid : public Texture {
	private:
		u32 tilesWide;
		u32 tilesTall;

		f32 widthFraction;
		f32 heightFraction;

	public:
		TileGrid(const char*, u32, u32, TextureParams = TextureParams());
		TileGrid(const char*, u32, TextureParams = TextureParams());

		void customProcess();

		f32* getSheet(u32, u32);
		f32* getSheet(u32);
	};
}

#endif
