
#ifndef CNGE_SVG_TAG
#define CNGE_SVG_TAG

#include <fstream>
#include "types.h"

namespace CNGE {
	class Tag {
	private:
		i32 numVariableNames;
		const char** variableNames;

	public:
		struct variableReturn {
			char* data;
			i32 id;
		};

		Tag(i32, const char**);
		Tag(const Tag&) = delete;

		auto readVariable(std::ifstream&) -> variableReturn;

		~Tag();
	};
}

#endif
