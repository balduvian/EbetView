
#include <iostream>

#include "cnge/load/resource.h"

#include "sceneManager.h"

namespace CNGE {
	SceneManager::SceneManager()
		: isLoading(false), scene(nullptr), loadScreen(nullptr) {}
	
	auto SceneManager::startLoading(CNFW::Input* input, std::unique_ptr<Scene>&& initialScene, std::unique_ptr<LoadScreen>&& initialLoadScreen) -> void {
		isLoading = true;

		/* set the scene and loadScreen ftom what we passed in */
		scene = std::move(initialScene);
		loadScreen = std::move(initialLoadScreen);

		scene->getBundle()->load();

		/* setup the load screen for the current window */
		loadScreen->resized(input->getWidth(), input->getHeight());
	}

	auto SceneManager::startLoadingQuick(CNFW::Input* input, std::unique_ptr<Scene>&& initialScene) -> void {
		isLoading = false;

		/* set the scene and loadScreen ftom what we passed in */
		scene = std::move(initialScene);

		scene->getBundle()->load(false).quickUpdate();
		
		scene->resized(input->getWidth(), input->getHeight());
		scene->start();
	}

	auto SceneManager::updateLoading(CNFW::Window* window, CNFW::Timing* timing) -> void {
		auto* input = window->getInput();
		
		/* resize the loading screen */
		if (input->getResized())
			loadScreen->resized(input->getWidth(), input->getHeight());

		/* advance loading along */
		auto *bundle = scene->getBundle();
		bundle->update();

		/* when the loading screen is done */
		if (bundle->getDone()) {
			/* setup the scene for the current window */
			scene->resized(input->getWidth(), input->getHeight());
			scene->start();
			
			isLoading = STATUS_UPDATING;

		} else {
			/* update the loading screen */
			loadScreen->update(input, timing);
			loadScreen->render(bundle->getCurrent(), bundle->getAlong(), bundle->getTotal());
		}
	}

	auto SceneManager::updateScene(CNFW::Window* window, CNFW::Timing* timing) -> void {
		auto* input = window->getInput();

		scene->setShouldRender(false);
		
		/* resize the scene with the window */
		if (input->getResized())
			scene->resized(input->getWidth(), input->getHeight());		
		
		scene->update(input, timing);

		if (scene->getShouldRender()) {
			scene->render();

			glFlush();
			
			//window->swap();
		}

		input->update();

		auto [newScene, newLoadScreen] = scene->switchScene();

		/* if the scene wants to switch */
		if (newScene != nullptr)
			/* if the scene has a load screen */
			if (newLoadScreen != nullptr)
				startLoading(input, std::move(newScene), std::move(newLoadScreen));
			else
				startLoadingQuick(input, std::move(newScene));
	}

	/// manages the updating of both loading screens and scenes
	/// updates and renders
	auto SceneManager::update(CNFW::Window* window, CNFW::Timing* timing) -> void {
		auto keepGoing = true;
		
		while(keepGoing) {
			/* keep track of which loading mode we are going to do */
			const auto lastLoading = isLoading;

			/* do the appropriate update for the loading mode */
			/* update which loading mode the */
			(lastLoading == STATUS_LOADING) ? updateLoading(window, timing) : updateScene(window, timing);

			/* run back through the loop with the other loading status if neccessary */
			keepGoing = (isLoading != lastLoading);
		}
	}

}
