
#include <functional>

#include "../cnge8/engine/window.h"
#include "../cnge8/engine/loop.h"

#include "res.h"
#include "scene/ViewScene.h"
#include "scene/sceneManager.h"
#include <windows.h>

#include "icon.h"

auto getInputFile() -> std::string {
	auto numArgs = 0;
	
	auto args = CommandLineToArgvW(GetCommandLineW(), &numArgs);
	auto ret = std::string();
	
	if (numArgs > 1) {
		/* if we have a first argument grab it */
		auto wideString = std::wstring(args[1]);
		ret = std::string(wideString.begin(), wideString.end());
	}

	LocalFree(args);

	return ret;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	/* init glfw and opengl */
	CNGE::Window::init();
	auto window = CNGE::Window(3, 3, true, true, "EbetView", CNGE::Window::getPrimaryMonitor(), false, true);
	CNGE::Window::wrangle();

	/* open enables */
	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	auto res = Game::IconResource(4);
	std::cout << res.getData() << std::endl;
	
	/* parse which file we are opening */
	auto inputFile = getInputFile();
	
	/* setup the scene no load screen */
	auto sceneManager = CNGE::SceneManager();
	sceneManager.startLoadingQuick(window.getInput(), std::make_unique<Game::ViewScene>(std::move(inputFile)));

	auto loop = CNGE::Loop(std::bind(&CNGE::Window::getShouldClose, window), [&](CNGE::Timing* timing) {
		window.poll();
		
		sceneManager.update(&window, timing);
		
	}, window.getRefreshRate());

	return 0;
}
