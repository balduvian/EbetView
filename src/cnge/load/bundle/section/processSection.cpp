
#include "processSection.h"

namespace CNGE {
	bool ProcessSection::skip() {
		return (*iterator)->getProcessStatus() == Resource::PROCESS_PROCESSED;
	}

	void ProcessSection::load() {
		(*iterator)->process();
	}

	ProcessSection::ProcessSection(LoadSection* next) : LoadSection(next) {}

}
