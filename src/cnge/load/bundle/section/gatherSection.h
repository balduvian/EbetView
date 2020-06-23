
#ifndef CNGE_GATHERSECTION
#define CNGE_GATHERSECTION

#include "../loadSection.h"

namespace CNGE {
	class GatherSection final : public LoadSection {
	private:
		bool wait() override;

		bool skip() override;

		void load() override;

	public:
		explicit GatherSection(LoadSection*);

	};

}

#endif
