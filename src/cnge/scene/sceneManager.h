
#ifndef CNGE_SCENE_MANAGER
#define CNGE_SCENE_MANAGER

#include <memory>

#include "scene.h"

#include "cnfw/window.h"
#include "cnfw/input.h"
#include "cnfw/timing.h"
#include "cnge/load/loadScreen.h"

namespace CNGE {
	class SceneManager {
	private:
		constexpr static bool STATUS_LOADING = true;
		constexpr static bool STATUS_UPDATING = false;
		
        bool isLoading;

		std::unique_ptr<Scene> scene;
		std::unique_ptr<LoadScreen> loadScreen;

		/* sub update funcitons */
		auto updateLoading(CNFW::Window*, CNFW::Timing*) -> void;
		auto updateScene(CNFW::Window*, CNFW::Timing*) -> void;
		
	public:
		SceneManager();

		auto startLoading(CNFW::Input*, std::unique_ptr<Scene>&&, std::unique_ptr<LoadScreen>&&) -> void;

		auto startLoadingQuick(CNFW::Input*, std::unique_ptr<Scene>&&) -> void;
		
		auto update(CNFW::Window*, CNFW::Timing*) -> void;
	};

}

#endif
