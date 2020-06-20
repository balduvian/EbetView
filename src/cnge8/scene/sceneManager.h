
#ifndef CNGE_SCENE_MANAGER
#define CNGE_SCENE_MANAGER

#include <memory>

#include "scene.h"
#include "../types.h"
#include "../engine/window.h"
#include "../load/loadScreen.h"

namespace CNGE {
	class SceneManager {
	private:
		constexpr static bool STATUS_LOADING = true;
		constexpr static bool STATUS_UPDATING = false;
		
        bool isLoading;

		std::unique_ptr<Scene> scene;
		std::unique_ptr<LoadScreen> loadScreen;

		/* sub update funcitons */
		auto updateLoading(Window*, Timing*) -> void;
		auto updateScene(Window*, Timing*) -> void;
		
	public:
		SceneManager();

		auto startLoading(Input*, std::unique_ptr<Scene>&&, std::unique_ptr<LoadScreen>&&) -> void;

		auto startLoadingQuick(Input*, std::unique_ptr<Scene>&&) -> void;
		
		auto update(Window*, Timing*) -> void;
	};

}

#endif
