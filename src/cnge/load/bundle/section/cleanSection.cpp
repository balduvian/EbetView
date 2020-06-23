
#include "cleanSection.h"

namespace CNGE {
	bool CleanSection::skip() {
		return (*iterator)->getGatherStatus() == Resource::GATHER_UNGATHERED;
	}

	void CleanSection::load() {
		(*iterator)->discard();
	}

	CleanSection::CleanSection(LoadSection* next) : LoadSection(next) {}

}
