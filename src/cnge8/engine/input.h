
#include "window.h"

#ifndef CNGE_INPUT
#define CNGE_INPUT

#include "GLFW/glfw3.h"
#include "../Types.h"

namespace CNGE {
	class Input {
	public:
		constexpr static u32
			BUTTON_RELEASED  = 0x00,
			BUTTON_UNPRESSED = 0x01,
			BUTTON_PRESSED   = 0x02,
			BUTTON_HELD      = 0x03,
			BUTTON_REPEAT    = 0x04;

		u32 getKeyPressed(i32);
		bool getKeyDown(i32);

		u32 getMousePressed(i32);
		bool getMouseDown(i32);
		
		f64 getCursorX();
		f64 getCursorY();

		bool getResized();

		u32 getWidth();
		u32 getHeight();
		
		void update();

		friend class Window;

		f32 getScrollX();
		f32 getScrollY();

	private:
		Input();

		void keyPressedUpdate(i32 key, i32 scancode, i32 action, i32 mods);

		void mousePressedUpdate(i32 button, i32 action, i32 mods);

		void mouseMovedUpdate(f64 x, f64 y);

		void scrollUpdate(f64 scrollX, f64 scrollY);

		u32 keysPressed[GLFW_KEY_LAST + 1];
		u32 mousePressed[GLFW_MOUSE_BUTTON_LAST + 1];

		f64 cursorX;
		f64 cursorY;

		f64 scrollX;
		f64 scrollY;

		bool resized;

		u32 width;
		u32 height;
	};
}

#endif
