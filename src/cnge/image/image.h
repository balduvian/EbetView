
#ifndef CNGE_IMAGE
#define CNGE_IMAGE

#include <png.h>
#include "types.h"

namespace CNGE {

	class Image {
	protected:
		i32 width;
		i32 height;
		FILE* file;
		png_struct* png;
		png_info* info;

		void endRead();
	public:
		Image(const char*);

		i32 getWidth();

		i32 getHeight();
	};

	class Image1D : public Image {
	private:
		u8* pixels;
	public:
		Image1D(const char*);

		u8* getPixels();

		~Image1D();
	};

	class Image2D : public Image {
	private:
		u8** pixels;
	public:
		Image2D(const char*);

		u8** getPixels();

		~Image2D();
	};
}

#endif
