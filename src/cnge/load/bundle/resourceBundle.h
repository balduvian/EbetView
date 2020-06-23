
#ifndef CNGE_ASSETSMANAGER
#define CNGE_ASSETSMANAGER

#include <initializer_list>
#include <types.h>
#include <vector>

#include "types.h"

#include "loadSection.h"
#include "section/cleanSection.h"
#include "section/gatherSection.h"
#include "section/processSection.h"
#include "section/unloadSection.h"

namespace CNGE {
	class ResourceBundle {
	private:
		GatherSection gatherSection;
		ProcessSection processSection;
		UnloadSection unloadSection;
		CleanSection cleanSection;

		/// stores the pointers to resources to load/unload ///
		std::vector<Resource*> resources;

		/// keeping track of the load ///
		i32 along;
		LoadSection* loadSection;
		bool clean;

		/// begins loading/unloading functions ///
		/// called publicly by load() and unload() ///
		ResourceBundle& setup(bool, LoadSection*);

		friend class LoadSection;
		friend class ProcessSection;

	public:
		/// constructing using a static initializer list ///
		ResourceBundle(std::initializer_list<Resource*>);

		/// you cannot copy the asset bundle ///
		ResourceBundle(const ResourceBundle&) = delete;
		ResourceBundle(const ResourceBundle&&) = delete;
		void operator=(const ResourceBundle&) = delete;

		/// the default for loading is to clean the loads ///
		ResourceBundle& load(bool = true);
		ResourceBundle& unload(bool = true);

		/// loading that blocks the thread ///
		void quickUpdate();

		/// call every frame to do multithreaded loading ///
		void update();

		/// getters ///
		Resource* getCurrent();
		i32 getAlong();
		i32 getTotal();

		bool getDone();

	};

}

#endif
