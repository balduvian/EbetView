
#include "UnloadSection.h"

namespace CNGE {
	bool UnloadSection::skip() {
		return (*iterator)->getProcessStatus() == Resource::PROCESS_UNPROCESSED;
	}

	void UnloadSection::load() {
		(*iterator)->unload();
	}

	UnloadSection::UnloadSection(LoadSection* next) : LoadSection(next) {}

}
