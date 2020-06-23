
#ifndef CNGE_CLEANSECTION
#define CNGE_CLEANSECTION

#include "../loadSection.h"

namespace CNGE {
	class CleanSection : public LoadSection {
	private:
		bool skip();

		void load();

	public:
		CleanSection(LoadSection*);

	};

}

#endif
