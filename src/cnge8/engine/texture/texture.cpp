
#include "GL/glew.h"
#include "GL/gl.h"

#include "png.h"

#include "texture.h"

#include <iostream>

namespace CNGE {
	float Texture::tileValues[4]{ 1, 1, 0, 0 };
	const float Texture::DEFAULT_TILE_VALUES[4]{ 1, 1, 0, 0 };

	/* loading */

	/// regular texture constructor
	/// without texture params, will set to default params
	Texture::Texture(const char* path, TextureParams params)
		: Resource(true), assetPath(path), assetImage(), width(), height(), texture(),
		horzWrap(params.horzWrap), vertWrap(params.vertWrap),
		minFilter(params.minFilter), magFilter(params.magFilter) {}

	void Texture::customGather() {
		assetImage = std::make_unique<Image1D>(assetPath);

		width = assetImage->getWidth();
		height = assetImage->getHeight();
	}

	void Texture::customProcess() {
		glCreateTextures(GL_TEXTURE_2D, 1, &texture);

		bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, horzWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vertWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, assetImage->getPixels());
	}

	void Texture::customDiscard() {
		assetImage = 0;
	}

	void Texture::customUnload() {
		glDeleteTextures(1, &texture);
	}

	/* use */

	void Texture::bind() {
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	void Texture::bind(i32 slot) {
		glActiveTexture(slot);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	int Texture::get() {
		return texture;
	}

	auto Texture::getImage() const -> Image1D* {
		return assetImage.get();
	}

	Texture::~Texture() {
		unload();
	}
}
