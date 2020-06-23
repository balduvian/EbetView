
#ifndef CNGE_STREAM_UTIL
#define CNGE_STREAM_UTIL

#include <fstream>
#include "types.h"

namespace CNGE {
	constexpr static i32 NO_OPTION_FOUND = -1;

	auto wordChar(char) -> bool;

	auto goUntil(std::ifstream&, char) -> void;

	auto nameMatch(std::ifstream&, i32, const char*[]) -> i32;

	auto stepBack(std::ifstream&) -> void;
}

#endif
