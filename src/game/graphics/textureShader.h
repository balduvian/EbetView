
#ifndef GAME_TEXTURE_SHADER
#define GAME_TEXTURE_SHADER

#include "../../cnge8/engine/shader.h"
#include "../../cnge8/Types.h"

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
