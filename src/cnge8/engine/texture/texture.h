
#ifndef CNGE_TEXTURE
#define CNGE_TEXTURE

#include "../../types.h"
#include "../../image/image.h"
#include "../../load/resource.h"
#include "textureParams.h"

namespace CNGE {
	class Texture : public Resource {
	public:
		Texture(const char*, TextureParams = TextureParams());

		const static float DEFAULT_TILE_VALUES[4];

		void bind(i32);
		void bind();

		int get();

		[[nodiscard]] auto getImage() const -> Image1D*;

		~Texture();

	protected:
		static float tileValues[4];

		virtual void customGather();
		virtual void customProcess();
		virtual void customDiscard();
		virtual void customUnload();

		u32 width;
		u32 height;

	private:
		const char* assetPath;
		std::unique_ptr<Image1D> assetImage;

		u32 texture;

		i32 horzWrap, vertWrap, minFilter, magFilter;
	};
}

#endif
