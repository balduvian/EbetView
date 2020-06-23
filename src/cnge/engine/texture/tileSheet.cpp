
#include "tileSheet.h"

namespace CNGE {
	TileSheet::TileSheet(const char* path, const u32 numPositions, const i32* positions, TextureParams params)
		: Texture(path, params), numPositions(numPositions), inputPositions(positions), calculatedPositions() {}

	void TileSheet::customProcess() {
		Texture::customProcess();

		calculatedPositions[0] = new f32[numPositions];
		calculatedPositions[1] = new f32[numPositions];
		calculatedPositions[2] = new f32[numPositions];
		calculatedPositions[3] = new f32[numPositions];

		auto widthFraction = 1._f32 / width;
		auto heightFraction = 1._f32 / height;

		auto inputPtr = inputPositions - 1;

		for (auto i = 0; i < numPositions; ++i) {
			calculatedPositions[0][i] =  widthFraction * *++inputPtr;
			calculatedPositions[1][i] = heightFraction * *++inputPtr;
			calculatedPositions[2][i] =  widthFraction * *++inputPtr;
			calculatedPositions[3][i] = heightFraction * *++inputPtr;
		}
	}

	f32* TileSheet::getSheet(u32 tile) {
		tileValues[0] = calculatedPositions[0][tile];
		tileValues[1] = calculatedPositions[1][tile];
		tileValues[2] = calculatedPositions[2][tile];
		tileValues[3] = calculatedPositions[3][tile];

		return tileValues;
	}

	void TileSheet::customUnload() {
		Texture::customUnload();

		delete[] calculatedPositions[0];
		delete[] calculatedPositions[1];
		delete[] calculatedPositions[2];
		delete[] calculatedPositions[3];
	}

	TileSheet::~TileSheet() {
		delete[] inputPositions;
	}
}
