
#ifndef CNGE_SVG
#define CNGE_SVG

#include <vector>
#include <stack>

#include "types.h"
#include "streamUtil.h"
#include "tag.h"

namespace CNGE {
	class SVGClass {
	public:
		i32 color;
	};

	class SVGTriangle {
	public:
		SVGClass* svgClass;
		f32 x0, y0, x1, y1, x2, y2;
	};

	class SVG {
	private:
		constexpr static i32 NUM_TAGS = 3;
		static const char* tagNames[NUM_TAGS];
		static Tag tags[NUM_TAGS];

		std::stack<Tag> tag;

		f32 width;
		f32 height;

		std::vector<SVGClass> classes;
		std::vector<SVGTriangle> svgTriangle;

		auto findTag(std::ifstream&, i32&) -> void;

	public:
		SVG(const char*);

		~SVG();
	};
}

#endif
