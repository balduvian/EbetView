
#include "streamUtil.h"
#include <iostream>

namespace CNGE {

	auto wordChar(char current) -> bool {
		auto lowerCase = current | 0x20;

		return !(lowerCase < 'a' || lowerCase > 'z');
	}

	auto goUntil(std::ifstream& stream, char limiter) -> void {
		auto current = char();

		do {
			current = stream.get();

			if (current == EOF)
				throw "end of file";
			
		} while (current != limiter);
	}

	auto nameMatch(std::ifstream& stream, i32 numOptions, const char* options[]) -> i32 {
		auto eliminated = new bool[numOptions]();

		auto numRemaining = numOptions;

		auto foundVariable = NO_OPTION_FOUND;
		auto matchIndex = 0;

		auto started = false;

		auto current = char();

		do {
			current = stream.get();

			if (wordChar(current)) {
				started = true;
				
			}
		} while (!started);

		auto inWord = true;

		do {
			if (numRemaining == 0) {
				if (!wordChar(current))
					inWord = false;

			} else {
				for (auto i = 0; i < numOptions; ++i) {
					if (!eliminated[i]) {
						/* the variable name's current char to compare */
						auto optionCurrent = options[i];
						auto charCurrent = optionCurrent[matchIndex];

						/* eliminate any names that don't match */
						if (charCurrent != current) {

							if (!wordChar(current)) {
								foundVariable = i;
								inWord = false;
							}

							eliminated[i] = true;
							--numRemaining;
						}
					}
				}

				++matchIndex;
			}

		} while (inWord && (current = stream.get(), true));

		delete[] eliminated;

		stepBack(stream);

		return foundVariable;
	}

	auto stepBack(std::ifstream& stream) -> void {
		stream.seekg(stream.tellg() - (std::streampos)1);
	}
}
