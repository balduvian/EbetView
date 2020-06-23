
#ifndef CNGE_RESOURCE
#define CNGE_RESOURCE

#include <thread>

#include <types.h>
#include "types.h"

namespace CNGE {
	class Resource {
	private:
		i32 gatherStatus;
		i32 processStatus;

		std::thread gatherThread;

		bool hasGather;

	protected:
		virtual void customGather() = 0; // Gather the required resources from disk
		virtual void customProcess() = 0; // Process the data retrieved from disk (e.g. sending to gpu)
		virtual void customDiscard() = 0; // Discard the data from disk once it's been processed or if the resource is to be unloaded
		virtual void customUnload() = 0; // Unload the resource

		auto smartDestroy() -> void;

	public:
		Resource(bool hasGather);

		constexpr static i32
			GATHER_UNGATHERED = 0,
			GATHER_GATHERING = 1,
			GATHER_GATHERED = 2,
			PROCESS_UNPROCESSED = 0,
			PROCESS_PROCESSED = 1;

		void gather();
		void quickGather();
		void joinThread();
		/// discard undoes gather
		void discard();
		
		void process();
		/// unload undoes process
		void unload();

		i32 getGatherStatus();
		i32 getProcessStatus();

		bool getHasGather();
	};

}

#endif
