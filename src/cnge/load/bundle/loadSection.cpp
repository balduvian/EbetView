
#include "loadSection.h"

//DEBUG
#include <iostream>

namespace CNGE {
	bool LoadSection::wait() {
		return false;
	}

	LoadSection::LoadSection(LoadSection* next) : next(next), iterator(), end() {}

	void LoadSection::start(BundleIt start, BundleIt end) {
		this->iterator = start;
		this->end = end;
	}

	void LoadSection::update() {
		if (wait())
			return;

		while (iterator < end && skip())
			++iterator;

		if (!getDone()) {
			load();
			return;
		}
	}

	bool LoadSection::getDone() {
		return iterator == end;
	}

	LoadSection* LoadSection::getNext() {
		return next;
	}

	Resource* LoadSection::getCurrent() {
		return *iterator;
	}

}
