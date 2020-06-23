
#include "gatherSection.h"

namespace CNGE {
	bool GatherSection::wait() {
		return (*iterator)->getGatherStatus() == Resource::GATHER_GATHERING;
	}

	bool GatherSection::skip() {
		return (!(*iterator)->getHasGather() || (*iterator)->getGatherStatus() == Resource::GATHER_GATHERED)
			&& ((*iterator)->joinThread(), true);
	}

	void GatherSection::load() {
		(*iterator)->gather();
	}

	GatherSection::GatherSection(LoadSection* next) : LoadSection(next) {}

}
