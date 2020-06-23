
#ifndef CNGE_TILE_SHEET
#define CNGE_TILE_SHEET

#include "texture.h"

#include "types.h"

namespace CNGE {
	class TileSheet : public Texture {
	private:
		u32 numPositions;
		const i32* inputPositions;

		f32* calculatedPositions[4];

	public:
		TileSheet(const char*, const u32, const i32*, TextureParams = TextureParams());

		void customProcess();
		void customUnload();

		f32* getSheet(u32);

		~TileSheet();
	};
}

#endif