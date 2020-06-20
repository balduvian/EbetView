#include <iostream>

#include "resource.h"

namespace CNGE {
	Resource::Resource(bool hasGather)
		: hasGather(hasGather), gatherStatus(GATHER_UNGATHERED), processStatus(PROCESS_UNPROCESSED), gatherThread() {}

	bool Resource::getHasGather() {
		return hasGather;
	}

	void Resource::gather() {
		// flags say we are not ready no more
		gatherStatus = GATHER_GATHERING;

		// startLoading up the gathering thread
		gatherThread = std::thread([this] {
			// do the actual gather
			customGather();

			// now after gathering we are ready
			gatherStatus = GATHER_GATHERED;
		});
	}

	void Resource::quickGather() {
		customGather();

		gatherStatus = GATHER_GATHERED;
	}
	
	/// assets manager deals with this after gathering is complete
	/// this is smart and won't crash on a non running thread
	void Resource::joinThread() {
		if (gatherThread.joinable()) {
			gatherThread.join();
		}
	}

	void Resource::process() {
		// single threaded, order doesn't necessarily matter
		customProcess();

		processStatus = PROCESS_PROCESSED;
	}

	void Resource::discard() {
		customDiscard();

		gatherStatus = GATHER_UNGATHERED;
	}

	void Resource::unload() {
		customUnload();

		processStatus = PROCESS_UNPROCESSED;
	}

	i32 Resource::getGatherStatus() {
		return gatherStatus;
	}

	i32 Resource::getProcessStatus() {
		return processStatus;
	}

	auto Resource::smartDestroy() -> void {
		if (processStatus == PROCESS_PROCESSED)
			customUnload();

		if (gatherStatus == GATHER_GATHERED)
			customDiscard();
	}

}
