
#include <exception>
#include "image.h"

namespace CNGE {

	Image1D::Image1D(const char* path) : Image(path) {
		auto rowBytes = png_get_rowbytes(png, info);

		pixels = new u8[rowBytes * height];

		// read in all rows into a 1D array

		auto pixelsPtr = pixels;		
		for (auto i = 0; i < height; ++i) {
			png_read_row(png, pixelsPtr, nullptr);
			pixelsPtr += rowBytes;
		}
			
		endRead();
	}

	Image2D::Image2D(const char* path) : Image(path) {
		auto rowBytes = png_get_rowbytes(png, info);

		// allocate a 2d array
		pixels = new u8 * [height];
		for (int y = 0; y < height; ++y)
			pixels[y] = new u8[rowBytes];

		// read all pixels all at once

		png_read_image(png, pixels);

		endRead();
	}

	Image::Image(const char* path) : file(nullptr) {
		// open the file of the image
		fopen_s(&file, path, "rb");

		/* check if the file exists */
		if (file == nullptr)
			throw std::exception("File not found");

		/* check if the file is a png */
		u8 sig[8];
		fread(sig, 1, 8, file);
		if (png_sig_cmp(sig, 0, 8))
			throw std::exception("Image not a PNG");
		
		// startLoading reading the file
		png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		info = png_create_info_struct(png);

		png_set_sig_bytes(png, 8);
		
		png_init_io(png, file);
		
		png_read_info(png, info);

		// get info about the image
		// and set width and height

		width = png_get_image_width(png, info);
		height = png_get_image_height(png, info);
		auto colorType = png_get_color_type(png, info);
		auto bitDepth = png_get_bit_depth(png, info);

		// convert the image into 8 bit rgba

		if (bitDepth == 16)
			png_set_strip_16(png);

		if (colorType == PNG_COLOR_TYPE_PALETTE)
			png_set_palette_to_rgb(png);

		if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
			png_set_expand_gray_1_2_4_to_8(png);

		if (png_get_valid(png, info, PNG_INFO_tRNS))
			png_set_tRNS_to_alpha(png);

		if (colorType == PNG_COLOR_TYPE_RGB || colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_PALETTE)
			png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

		if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_gray_to_rgb(png);
		
		png_read_update_info(png, info);
	}

	i32 Image::getWidth() {
		return width;
	}

	i32 Image::getHeight() {
		return height;
	}

	void Image::endRead() {
		png_destroy_read_struct(&png, &info, nullptr);
		fclose(file);
	}

	u8* Image1D::getPixels() {
		return pixels;
	}

	u8** Image2D::getPixels() {
		return pixels;
	}

	Image1D::~Image1D() {
		delete[] pixels;
	}

	Image2D::~Image2D() {
		for (auto i = 0; i < height; ++i)
			delete[] pixels[i];
		
		delete[] pixels;
	}

}
