
#include "window.h"
#include <exception>
#include <string>

#include "cnge/math/math.h"

namespace CNFW {

	auto Window::windowProc(HWND const window, UINT const message, WPARAM const wParam, LPARAM const lParam) -> LRESULT {
		static auto paintStruct = PAINTSTRUCT{};

		switch (message) {
		case WM_PAINT: {
			BeginPaint(window, &paintStruct);
			EndPaint(window, &paintStruct);
		} return 0;

		case WM_SIZE: {
			input.resized = true;

			input.width = LOWORD(lParam);
			input.height = HIWORD(lParam);
		} return 0;

		case WM_MOUSEWHEEL: {
			input.scroll = CNGE::sign(GET_WHEEL_DELTA_WPARAM(wParam));
		} return 0;

		case WM_KEYDOWN: {
			auto const lastState = (lParam >> 30) & 1;

			if (lastState == 0 && wParam < Input::NUM_KEYS)
				input.keys[wParam] = Input::BUTTON_PRESSED;
		} return 0;

		case WM_KEYUP: {
			if (wParam < Input::NUM_KEYS)
				input.keys[wParam] = Input::BUTTON_RELEASED;
		} return 0;
			
		case WM_LBUTTONDOWN: {
			input.buttons[Input::BUTTON_LEFT] = Input::BUTTON_PRESSED;
		} return 0;

		case WM_MBUTTONDOWN: {
			input.buttons[Input::BUTTON_MIDDLE] = Input::BUTTON_PRESSED;
		} return 0;
			
		case WM_RBUTTONDOWN: {
			input.buttons[Input::BUTTON_RIGHT] = Input::BUTTON_PRESSED;
		} return 0;

		case WM_LBUTTONUP: {
			input.buttons[Input::BUTTON_LEFT] = Input::BUTTON_RELEASED;
		} return 0;

		case WM_MBUTTONUP: {
			input.buttons[Input::BUTTON_MIDDLE] = Input::BUTTON_RELEASED;
		} return 0;
			
		case WM_RBUTTONUP: {
			input.buttons[Input::BUTTON_RIGHT] = Input::BUTTON_RELEASED;
		} return 0;
			
		case WM_MOUSEMOVE: {
			auto mouseX = i32(LOWORD(lParam));
			auto mouseY = i32(HIWORD(lParam));
			/* Win32 is pretty braindead about the x, y position that
			   it returns when the mouse is off the left or top edge
			   of the window (due to them being unsigned). therefore,
			   roll the Win32's 0..2^16 pointer co-ord range to the
			   more amenable (and useful) 0..+/-2^15. */
			if (mouseX & 1 << 15) mouseX -= (1 << 16);
			if (mouseY & 1 << 15) mouseY -= (1 << 16);

			input.mouseX = mouseX;
			input.mouseY = mouseY;
			
		} return 0;

		case WM_PALETTECHANGED:
			if (window == HWND(wParam))
				return 0;
			/* fall through to WM_QUERYNEWPALETTE */

		case WM_QUERYNEWPALETTE: {
			if (palette) {
				UnrealizeObject(palette);
				SelectPalette(deviceContext, palette, false);
				RealizePalette(deviceContext);
				
				return true;
			}
		} return 0;

		case WM_DPICHANGED: {
			auto* const prcNewWindow = reinterpret_cast<RECT*>(lParam);
			
			SetWindowPos(
				window,
				nullptr,
				prcNewWindow->left,
				prcNewWindow->top,
				prcNewWindow->right - prcNewWindow->left,
				prcNewWindow->bottom - prcNewWindow->top,
				SWP_NOZORDER | SWP_NOACTIVATE
			);
		} return 0;

		case WM_CLOSE: {
			shouldClose = true;
		} return 0;

		default:
			return DefWindowProc(window, message, wParam, lParam);
		}
	}
	
	auto Window::createOpenGLWindow(const wchar_t* title, const i32 x, const i32 y, const i32 width, const i32 height, const i32 iconID, const BYTE type, const DWORD flags) -> void {
		/* grab the current instance */
		instance = GetModuleHandle(nullptr);

		/* create the type of our opengl window */
		auto windowClass = WNDCLASS {
			.style = CS_OWNDC,
			.lpfnWndProc = WNDPROC(windowProc),
			.cbClsExtra = 0,
			.cbWndExtra = 0,
			.hInstance = instance,
			.hIcon = LoadIcon(instance, MAKEINTRESOURCE(iconID)),
			.hCursor = LoadCursor(nullptr, IDC_ARROW),
			.hbrBackground = nullptr,
			.lpszMenuName = nullptr,
			.lpszClassName = L"OpenGL"
		};

		if (!RegisterClass(&windowClass))
			throw std::exception("RegisterClass() failed: Cannot register window class.");

		/* create the window */
		window = CreateWindow(L"OpenGL", title, WS_OVERLAPPEDWINDOW |
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			x, y, width, height, nullptr, nullptr, instance, nullptr);

		if (!window)
			throw std::exception("CreateWindow() failed:  Cannot create a window.");

		/* get the device context */
		deviceContext = GetDC(window);

		/* set the pixel format */
		auto pixelFormatDesc = PIXELFORMATDESCRIPTOR {
			.nSize = sizeof(PIXELFORMATDESCRIPTOR),
			.nVersion = 1,
			.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | flags,
			.iPixelType = type,
			.cColorBits = 32,
			.cDepthBits = 32
		};

		const auto pixelFormat = ChoosePixelFormat(deviceContext, &pixelFormatDesc);
		
		if (!pixelFormat)
			throw std::exception("Cannot find a suitable pixel format.");

		if (!SetPixelFormat(deviceContext, pixelFormat, &pixelFormatDesc))
			throw std::exception("Cannot set format specified.");

		DescribePixelFormat(deviceContext, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelFormatDesc);

		if (pixelFormatDesc.dwFlags & PFD_NEED_PALETTE || pixelFormatDesc.iPixelType == PFD_TYPE_COLORINDEX) {
			auto n = 1 << pixelFormatDesc.cColorBits;
			
			if (n > 256)
				n = 256;
			
			auto* logPalette = static_cast<LOGPALETTE*>(malloc(sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * n));
			
			memset(logPalette, 0, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * n);
			logPalette->palVersion = 0x300;
			logPalette->palNumEntries = n;

			GetSystemPaletteEntries(deviceContext, 0, n, &logPalette->palPalEntry[0]);

			/* if the pixel type is RGBA, then we want to make an RGB ramp,
			   otherwise (color index) set individual colors. */
			if (pixelFormatDesc.iPixelType == PFD_TYPE_RGBA) {
				const auto redMask = (1 << pixelFormatDesc.cRedBits) - 1;
				const auto greenMask = (1 << pixelFormatDesc.cGreenBits) - 1;
				const auto blueMask = (1 << pixelFormatDesc.cBlueBits) - 1;
		
				/* fill in the entries with an RGB color ramp. */
				for (auto i = 0; i < n; ++i) {
					logPalette->palPalEntry[i].peRed =
						(((i >> pixelFormatDesc.cRedShift) & redMask) * 255) / redMask;
					logPalette->palPalEntry[i].peGreen =
						(((i >> pixelFormatDesc.cGreenShift) & greenMask) * 255) / greenMask;
					logPalette->palPalEntry[i].peBlue =
						(((i >> pixelFormatDesc.cBlueShift) & blueMask) * 255) / blueMask;
					logPalette->palPalEntry[i].peFlags = 0;
				}
				
			} else {
				logPalette->palPalEntry[0].peRed = 0;
				logPalette->palPalEntry[0].peGreen = 0;
				logPalette->palPalEntry[0].peBlue = 0;
				logPalette->palPalEntry[0].peFlags = PC_NOCOLLAPSE;
				logPalette->palPalEntry[1].peRed = 255;
				logPalette->palPalEntry[1].peGreen = 0;
				logPalette->palPalEntry[1].peBlue = 0;
				logPalette->palPalEntry[1].peFlags = PC_NOCOLLAPSE;
				logPalette->palPalEntry[2].peRed = 0;
				logPalette->palPalEntry[2].peGreen = 255;
				logPalette->palPalEntry[2].peBlue = 0;
				logPalette->palPalEntry[2].peFlags = PC_NOCOLLAPSE;
				logPalette->palPalEntry[3].peRed = 0;
				logPalette->palPalEntry[3].peGreen = 0;
				logPalette->palPalEntry[3].peBlue = 255;
				logPalette->palPalEntry[3].peFlags = PC_NOCOLLAPSE;
			}

			palette = CreatePalette(logPalette);
			
			if (palette) {
				SelectPalette(deviceContext, palette, FALSE);
				RealizePalette(deviceContext);
			}

			free(logPalette);
		}
	}

	HWND Window::window = nullptr;
	HINSTANCE Window::instance = nullptr;
	HDC Window::deviceContext = nullptr;
	HGLRC Window::glContext = nullptr;
	HPALETTE Window::palette = nullptr;

	Input Window::input{};
	bool Window::shouldClose = false;
	
	Window::Window(const wchar_t* title, i32 x, i32 y, i32 width, i32 height, i32 iconID) {
		/* get window and instance and context */
		createOpenGLWindow(title, x, y, width, height, iconID, PFD_TYPE_RGBA, PFD_DOUBLEBUFFER);

		glContext = wglCreateContext(deviceContext);
		wglMakeCurrent(deviceContext, glContext);

		ShowWindow(window, SW_SHOW);
	}
	
	auto Window::setIcon(i32 const id) -> void {
		auto* const icon = LoadIcon(instance, MAKEINTRESOURCE(id));
		SendMessageA(window, WM_SETICON, 1, LPARAM(icon));
	}

	auto Window::poll() -> void {
		auto message = MSG{};

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	auto Window::swap() -> void {
		SwapBuffers(deviceContext);
	}

	auto Window::getShouldClose() -> bool {
		return shouldClose;
	}

	auto Window::getInput() -> Input* {
		return &input;
	}

	auto Window::getWindow() -> HWND {
		return window;
	}

	auto Window::getScreenWidth() -> i32 {
		return GetSystemMetrics(SM_CXSCREEN);
	}

	auto Window::getScreenHeight() -> i32 {
		return GetSystemMetrics(SM_CYSCREEN);
	}

	auto Window::close() -> void {
		wglMakeCurrent(nullptr, nullptr);
		ReleaseDC(window, deviceContext);
		wglDeleteContext(glContext);
		DestroyWindow(window);
	}
}
