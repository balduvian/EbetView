
#include "cnfw/window.h"
#include "cnfw/loop.h"
#include "cnge/scene/sceneManager.h"

#include "scene/viewScene.h"

auto getInputFile() -> std::string {
	auto numArgs = 0;
	
	auto* const args = CommandLineToArgvW(GetCommandLine(), &numArgs);
	auto ret = std::string();
	
	if (numArgs > 1) {
		/* if we have a first argument grab it */
		auto wideString = std::wstring(args[1]);
		ret = std::string(wideString.begin(), wideString.end());
	}

	LocalFree(args);

	return ret;
}

auto WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) -> i32 {
	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
	SetProcessDPIAware();
	
	/* init glfw and opengl */
	auto screenWidth = CNFW::Window::getScreenWidth();
	auto screenHeight = CNFW::Window::getScreenHeight();
	
	auto width = screenWidth / 2;
	auto height = screenHeight / 2;
	
	auto window = CNFW::Window(L"EbetView", width / 2, height / 2, width, height, 420);
	glewInit();
	
	/* opengl enables */
	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	/* parse which file we are opening */
	auto inputFile = getInputFile();
	
	/* setup the scene no load screen */
	auto sceneManager = CNGE::SceneManager();
	sceneManager.startLoadingQuick(window.getInput(), std::make_unique<Game::ViewScene>(std::move(inputFile)));

	auto loop = CNFW::Loop(60);

	loop.begin([&window]() { return window.getShouldClose(); }, [&window, &sceneManager](auto* timing) {
		window.poll();

		sceneManager.update(&window, timing);
	});

	return 0;
}
