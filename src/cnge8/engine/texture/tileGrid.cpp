
#include "tileGrid.h"

namespace CNGE {
	TileGrid::TileGrid(const char* path, u32 tilesWide, u32 tilesTall, TextureParams params)
	: Texture(path, params), tilesWide(tilesWide), tilesTall(tilesTall), widthFraction(), heightFraction() {}

	TileGrid::TileGrid(const char* path, u32 tilesWide, TextureParams params)
	: Texture(path, params), tilesWide(tilesWide), tilesTall(1), widthFraction(), heightFraction() {}

	void TileGrid::customProcess() {
		Texture::customProcess();

		widthFraction = 1._f32 / tilesWide;
		heightFraction = 1._f32 / tilesTall;
	}

	f32* TileGrid::getSheet(u32 x, u32 y) {
		tileValues[0] = widthFraction;
		tileValues[1] = heightFraction;
		tileValues[2] = x * widthFraction;
		tileValues[3] = y * heightFraction;

		return tileValues;
	}

	f32* TileGrid::getSheet(u32 x) {
		tileValues[0] = widthFraction;
		tileValues[1] = 1;
		tileValues[2] = x * widthFraction;
		tileValues[3] = 0;

		return tileValues;
	}

}
