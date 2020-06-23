
#ifndef EBETVIEW_TEXTURE_SHADER
#define EBETVIEW_TEXTURE_SHADER

#include "cnge/engine/shader.h"
#include "types.h"

namespace Game {
	class TextureShader : public CNGE::Shader {
	private:
		i32 colorLoc = 0;
		i32 texModifLoc = 0;
		
	public:
		TextureShader();

		auto getUniforms() -> void override;

		auto giveParams(f32, f32, f32, f32, f32[]) -> void;

		auto giveParams(f32, f32, f32, f32) -> void;

		auto giveParams() -> void;
	};
}

#endif
