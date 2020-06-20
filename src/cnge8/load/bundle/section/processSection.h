
#ifndef CNGE_PROCESSSECTION
#define CNGE_PROCESSSECTION

#include "../LoadSection.h"

namespace CNGE {
	class ProcessSection : public LoadSection {
	private:
		bool skip();

		void load();

	public:
		ProcessSection(LoadSection*);

	};

}

#endif
