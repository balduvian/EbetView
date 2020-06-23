
#include <vector>
#include <stdexcept>

#include "resourceBundle.h"

namespace CNGE {
	ResourceBundle& ResourceBundle::setup(bool clean, LoadSection* initialMode) {
		/* we can only set up if we're not already loading */
		if (loadSection != nullptr)
			throw std::runtime_error("cannot setup, loading is already in progress");

		/* set up if we are going to clean */
		this->clean = clean;

		/* we haven't loaded anything yet */
		along = 0;

		/* startLoading loading by setting the initial mode */
		loadSection = initialMode;
		loadSection->start(resources.begin(), resources.end());

		return *this;
	}

	ResourceBundle::ResourceBundle(std::initializer_list<Resource*> list)
		: resources(list), loadSection(nullptr), along(0), clean(),
		gatherSection(&processSection),
		processSection(&cleanSection),
		unloadSection(&cleanSection),
		cleanSection(nullptr) {}

	ResourceBundle& ResourceBundle::load(bool clean) {
		/* gathering is the first stage for loading */
		return setup(clean, &gatherSection);
	}

	ResourceBundle& ResourceBundle::unload(bool clean) {
		/* unloading is the first stage for unloading */
		return setup(clean, &unloadSection);
	}

	void ResourceBundle::quickUpdate() {
		if (loadSection == nullptr)
			throw std::runtime_error("updated when loading has not started");

		/* if we are loading */
		if (loadSection == &gatherSection) {
			for (auto *resource : resources) {
				/* gather */
				if (resource->getHasGather() && resource->getGatherStatus() == Resource::GATHER_UNGATHERED) {
					resource->gather();
					resource->joinThread();
				}

				/* process */
				if (resource->getProcessStatus() == Resource::PROCESS_UNPROCESSED)
					resource->process();

				/* if we have set to clean */
				if (clean)
					resource->discard();
			}

			/* if we are unloading */
		}
		else {
			for (auto *resource : resources) {
				/* unload */
				if (resource->getProcessStatus() == Resource::PROCESS_PROCESSED)
					resource->unload();

				/* if we have set to clean */
				if (clean && resource->getGatherStatus() == Resource::GATHER_GATHERED)
					resource->discard();
			}
		}

		/* make sure we are done */
		loadSection = nullptr;
	}

	void ResourceBundle::update() {
		if (!loadSection)
			throw std::runtime_error("updated when loading has not started");

		/* continue going through the load sections */
		/* until we find a load we actually have to do */
		/* this prevents hanging out of range done load sections */

		if (loadSection->getDone()) {
			loadSection = loadSection->getNext();

			if (loadSection)
				loadSection->start(resources.begin(), resources.end());
		}

		if (!loadSection->getDone()) {
			loadSection->update();

			if (loadSection->getDone()) {
				loadSection = loadSection->getNext();

				if (loadSection)
					loadSection->start(resources.begin(), resources.end());
			}
		}
	}

	Resource* ResourceBundle::getCurrent() {
		return loadSection->getCurrent();
	}

	i32 ResourceBundle::getAlong() {
		return along;
	}

	i32 ResourceBundle::getTotal() {
		return resources.size();
	}

	bool ResourceBundle::getDone() {
		return !loadSection;
	}

}
