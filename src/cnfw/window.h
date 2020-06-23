
#ifndef CNFW_WINDOW
#define CNFW_WINDOW

#include <windows.h>

#undef near
#undef far

#include <GL/glew.h>
#include <GL/gl.h>

#include "input.h"
#include "types.h"

namespace CNFW {
	class Window {
	private:
		static HWND window;
		static HINSTANCE instance;
		static HDC deviceContext;
		static HGLRC glContext;
		static HPALETTE palette;

		static Input input;
		static bool shouldClose;
		
		static auto windowProc(HWND, UINT, WPARAM, LPARAM) -> LRESULT;

		static auto createOpenGLWindow(const wchar_t*, i32, i32, i32, i32, i32, BYTE, DWORD) -> void;
		
		friend class Loop;

	public:
		Window(const wchar_t*, i32, i32, i32, i32, i32);

		/* no copying the window */
		auto operator=(const Window&) -> void = delete;
		Window(const Window&) = delete;

		auto setIcon(i32) -> void;

		auto poll() -> void;
		auto swap() -> void;

		[[nodiscard]] auto getShouldClose() -> bool;
		[[nodiscard]] auto getInput() -> Input*;
		[[nodiscard]] auto getWindow() -> HWND;
		
		[[nodiscard]] static auto getScreenWidth() -> i32;
		[[nodiscard]] static auto getScreenHeight() -> i32;
		
		auto close() -> void;
	};
}

#endif
