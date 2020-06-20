
#ifndef CNGE_LOADSECTION
#define CNGE_LOADSECTION

#include "types.h"

namespace CNGE {
	class LoadSection {
	private:
		virtual bool wait();

		virtual bool skip() = 0;

		virtual void load() = 0;

		LoadSection* next;

	protected:
		BundleIt iterator;
		BundleIt end;

	public:
		virtual ~LoadSection() = default;
		LoadSection(LoadSection*);

		void start(BundleIt, BundleIt);

		void update();
		
		bool getDone();

		LoadSection* getNext();

		Resource* getCurrent();

	};

}

#endif
