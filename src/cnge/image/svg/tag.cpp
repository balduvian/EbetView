
#include "tag.h"
#include "streamUtil.h"
#include <iostream>

namespace CNGE {
	Tag::Tag(i32 numVariableNames, const char** variableNames)
		: numVariableNames(numVariableNames), variableNames(variableNames) {}

	auto Tag::readVariable(std::ifstream& stream) -> variableReturn {
		auto foundVariable = nameMatch(stream, numVariableNames, variableNames);

		if (foundVariable != NO_OPTION_FOUND) {
			/* go until the first character of the value */
			goUntil(stream, '=');
			goUntil(stream, '"');

			/* find the end of the variable to get length */
			auto start = stream.tellg();
			goUntil(stream, '"');
			auto length = (stream.tellg() - start) - 1;

			/* read in the variable to a buffer */
			auto data = new char[length + 1];
			stream.seekg(start);

			/* we read the last quote into data */
			/* then replace it with null terminator */
			stream.read(data, length + 1);
			data[length] = '\0';

			return { data, foundVariable };
		}
	}

	Tag::~Tag() {
		delete[] variableNames;
	}
}
