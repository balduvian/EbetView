
#include <iostream>

#include "input.h"

namespace CNGE {
	Input::Input(): keysPressed(), mousePressed(), cursorX(), cursorY(), width(), height(), resized(), scrollX(), scrollY() {}

	u32 Input::getKeyPressed(i32 key) {
		return keysPressed[key];
	}

	bool Input::getKeyDown(i32 key) {
		return keysPressed[key] == BUTTON_PRESSED || keysPressed[key] == BUTTON_HELD || keysPressed[key] == BUTTON_REPEAT;
	}

	u32 Input::getMousePressed(i32 button) {
		return mousePressed[button];
	}

	bool Input::getMouseDown(i32 button) {
		return mousePressed[button] == BUTTON_PRESSED || mousePressed[button] == BUTTON_HELD || mousePressed[button] == BUTTON_REPEAT;
	}

	f64 Input::getCursorX() {
		return cursorX;
	}

	f64 Input::getCursorY() {
		return cursorY;
	}

	void Input::keyPressedUpdate(i32 key, i32 scancode, i32 action, i32 mods) {
		if (key >= 0 && key <= GLFW_KEY_LAST) {
			if (action == GLFW_PRESS)
				keysPressed[key] = BUTTON_PRESSED;

			else if (action == GLFW_RELEASE)
				keysPressed[key] = BUTTON_RELEASED;

			else if (action == GLFW_REPEAT)
				keysPressed[key] = BUTTON_REPEAT;
		}
	}

	void Input::mousePressedUpdate(i32 button, i32 action, i32 mods) {
		if (button >= 0 && button <= GLFW_MOUSE_BUTTON_LAST) {
			if (action == GLFW_PRESS)
				mousePressed[button] = BUTTON_PRESSED;

			else if (action == GLFW_RELEASE)
				mousePressed[button] = BUTTON_RELEASED;

			else if (action == GLFW_REPEAT)
				mousePressed[button] = BUTTON_REPEAT;
		}
	}

	void Input::scrollUpdate(f64 x, f64 y) {
		scrollX = x;
		scrollY = y;
	}

	void Input::mouseMovedUpdate(f64 x, f64 y) {
		cursorX = x;
		cursorY = y;
	}

	bool Input::getResized() {
		return resized;
	}

	u32 Input::getWidth() {
		return width;
	}

	u32 Input::getHeight() {
		return height;
	}

	f32 Input::getScrollX() {
		return scrollX;
	}

	f32 Input::getScrollY() {
		return scrollY;
	}

	void Input::update() {
		resized = false;
		scrollX, scrollY = 0;

		for (u32 i = 0; i < GLFW_KEY_LAST; ++i) {
			if (keysPressed[i] == BUTTON_RELEASED || keysPressed[i] == BUTTON_PRESSED)
				++keysPressed[i];
			else if (keysPressed[i] == BUTTON_REPEAT)
				--keysPressed[i];
		}
		for (u32 i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i) {
			if (mousePressed[i] == BUTTON_RELEASED || mousePressed[i] == BUTTON_PRESSED)
				++mousePressed[i];
			else if (mousePressed[i] == BUTTON_REPEAT)
				--mousePressed[i];
		}
	}
}
