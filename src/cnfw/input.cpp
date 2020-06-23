
#include "input.h"

namespace CNFW {
	Input::Input() : mouseX(),
		mouseY(), scroll(),
		buttons(), keys(),
		width(), height(), resized() {}

	/**
	* IMPORTANT!
	*
	* call update on the input BEFORE you poll the window
	* or AFTER you update the game
	*
	* polling the window directly sets state to be used by
	* the game. Calling update in between the polling and the game
	* will cause some states to be missed.
	*/
	auto Input::update() -> void {
		resized = false;
		scroll = 0;
		
		for (auto& key : keys)
			if (key == BUTTON_PRESSED) key = BUTTON_HELD;

		for (auto& button : buttons)
			if (button == BUTTON_PRESSED) button = BUTTON_HELD;
	}
	
	auto Input::getButtonPressed(const i32 button) const -> bool {
		return buttons[button] == BUTTON_PRESSED;
	}
	
	auto Input::getButtonDown(const i32 button) const -> bool {
		return buttons[button] == BUTTON_HELD || buttons[button] == BUTTON_PRESSED;
	}
	
	auto Input::getKeyPressed(const i32 keyCode) const -> bool {
		return keys[keyCode] == BUTTON_PRESSED;
	}
	
	auto Input::getKeyDown(const i32 keyCode) const -> bool {
		return keys[keyCode] == BUTTON_HELD || keys[keyCode] == BUTTON_PRESSED;
	}
	
	auto Input::getMouseX() const -> i32 {
		return mouseX;
	}
	
	auto Input::getMouseY() const -> i32 {
		return mouseY;
	}
	
	auto Input::getScroll() const -> i32 {
		return scroll;
	}
	
	auto Input::getWidth() const -> i32 {
		return width;
	}
	
	auto Input::getHeight() const -> i32 {
		return height;
	}
	
	auto Input::getResized() const -> bool {
		return resized;
	}

}
