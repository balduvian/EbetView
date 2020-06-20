
#ifndef CNGE_UNLOADSECTION
#define CNGE_UNLOADSECTION

#include "../LoadSection.h"

namespace CNGE {
	class UnloadSection : public LoadSection {
	private:
		bool skip();

		void load();

	public:
		UnloadSection(LoadSection*);

	};

}

#endif
