
#include "../../cnge8/engine/texture/texture.h"
#include "textureShader.h"

namespace Game {
	constexpr static const char* VERTEX_SHADER =
		"#version 330 core\n"
		"layout(location = 0) in vec3 vertex;"
		"layout(location = 1) in vec2 texCoord;"
		"uniform mat4 model;"
		"uniform mat4 projView;"
		"uniform vec4 texModif;"
		"out vec2 texPass;"
		"void main() {"
		"texPass = (texCoord * texModif.xy) + texModif.zw;"
		"gl_Position = (projView * model) * vec4(vertex, 1);"
		"}";

	constexpr static const char* FRAGMENT_SHADER =
		"#version 330 core\n"
		"uniform sampler2D tex;"
		"uniform vec4 inColor;"
		"in vec2 texPass;"
		"out vec4 color;"
		"void main() {"
		"color = inColor * texture(tex, texPass);"
		"}";
	
	TextureShader::TextureShader() : Shader(false, VERTEX_SHADER, FRAGMENT_SHADER) {};

	auto TextureShader::getUniforms() -> void {
		colorLoc = getUniform("inColor");
		texModifLoc = getUniform("texModif");
	}

	auto TextureShader::giveParams(f32 r, f32 g, f32 b, f32 a, f32 texModif[]) -> void {
		giveVector4(colorLoc, r, g, b, a);
		giveVector4(texModifLoc, texModif);
	}

	auto TextureShader::giveParams(f32 r, f32 g, f32 b, f32 a) -> void {
		giveVector4(colorLoc, r, g, b, a);
		giveVector4(texModifLoc, CNGE::Texture::DEFAULT_TILE_VALUES);
	}

	auto TextureShader::giveParams() -> void {
		giveVector4(colorLoc, 1, 1, 1, 1);
		giveVector4(texModifLoc, CNGE::Texture::DEFAULT_TILE_VALUES);
	}
}
