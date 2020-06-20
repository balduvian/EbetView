
#include "svg.h"
#include <iostream>

namespace CNGE {
	const char* SVG::tagNames[NUM_TAGS]{
		"svg",
		"style",
		"polygon"
	};

	Tag SVG::tags[NUM_TAGS] = {
		{1, new const char* [1] {"viewBox"} },
		{0, new const char* [0] {} },
		{2, new const char* [2] {"class", "points"} }
	};

	auto SVG::findTag(std::ifstream& stream, i32& stackDepth) -> void {
		goUntil(stream, '<');
		++ stackDepth;
	}

	SVG::SVG(const char* filePath) {
		try {
			auto stream = std::ifstream(filePath);

			i32 stackDepth = 0;

			//find our first tag
			findTag(stream, stackDepth);

			while (stackDepth > 0) {
				auto tagIndex = nameMatch(stream, NUM_TAGS, tagNames);
				if (tagIndex != NO_OPTION_FOUND) {
					auto [index, data] = tags[tagIndex].readVariable(stream);
				}
			}
		}
		catch (const char*) {
			std::cout << "svg parsing failed";
		}
	}

	SVG::~SVG() {

	}
}

