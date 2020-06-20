
#include <GL\glew.h>
#include <gl/gl.h>

#include <string>
#include <iostream>

#include "window.h"

namespace CNGE {
	/// initializes GLFW
	/// you must call this before any other functions
	void Window::init() {
		// and setup error reporting
		glfwSetErrorCallback([](auto error, auto description) {
			std::cout << "GLFW ERROR CODE " << error << " | " << description << std::endl;
			exit(-1);
		});

		// initialize
		if (!glfwInit()) {
			std::cout << "glfw failed to initialize" << std::endl;
			exit(-1);
		}
	}

	/// throws if glew does not initialize
	void Window::wrangle() {
		glewExperimental = true;

		if (glewInit() != GLEW_OK) {
			std::cout << "glew failed to initialize" << std::endl;
			exit(-1);
		}
	}	

	/// returns an array of pointers to each monitor
	/// the monitor count is passed in and will be sent out
	GLFWmonitor** Window::getMonitors(int& monitorCount) {
		return glfwGetMonitors(&monitorCount);
	}

	/// returns the primary monitor for simple monitor getting
	GLFWmonitor* Window::getPrimaryMonitor() {
		return glfwGetPrimaryMonitor();
	}

	/// creates the window
	/// call more methods after this to modify window
	Window::Window(
		int majorVersion,
		int minorVersion,
		bool resizable,
		bool decorated,
		const char* title,
		GLFWmonitor* monitor,
		bool full,
		bool vsync
	) : full(full), monitor(monitor) {
		// set hints
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, resizable);
		glfwWindowHint(GLFW_DECORATED, decorated);

		// get the video mode from the monitor you pass in
		videoMode = glfwGetVideoMode(monitor);

		// get dimensions from the vidmode used for calculations
		auto vWidth = videoMode->width;
		auto vHeight = videoMode->height;

		// set the positions of the window to center inside the monitor
		width = full ? vWidth : vWidth / 2;
		height = full ? vHeight : vHeight / 2;
		x = vWidth / 4;
		y = vHeight / 4;

		// create the window, 
		window = glfwCreateWindow(width, height, title, full ? monitor : nullptr, nullptr);

		// if the window failed to init
		if (window == nullptr) {
			std::cout << "window failed to initialize" << std::endl;
			exit(-1);
		}

		// set the context
		glfwMakeContextCurrent(window);
		glfwSwapInterval(vsync);

		glfwShowWindow(window);
		glfwFocusWindow(window);

		// store this into glfw, "thanks c libraries" -slab 2019
		glfwSetWindowUserPointer(window, this);

		auto resizeCallback = [](auto window, auto width, auto height) {
			// grab this window class
			auto thisPtr = (Window*)glfwGetWindowUserPointer(window);

			// first set internal dimension values
			thisPtr->width = width;
			thisPtr->height = height;

			// make it accessible to input
			thisPtr->input.resized = true;
			thisPtr->input.width = width;
			thisPtr->input.height = height;
		};

		// do resize right away

		resizeCallback(window, width, height);

		glfwSetWindowSizeCallback(window, resizeCallback);

		/* 
		 * setup callbacks that get deligated to input
		 */

		glfwSetKeyCallback(window, [](auto window, auto key, auto scancode, auto action, auto mods) {
			((Window*)glfwGetWindowUserPointer(window))->input.keyPressedUpdate(key, scancode, action, mods);
		});

		glfwSetCursorPosCallback(window, [](auto window, auto x, auto y) {
			((Window*)glfwGetWindowUserPointer(window))->input.mouseMovedUpdate(x, y);
		});

		glfwSetMouseButtonCallback(window, [](auto window, auto button, auto action, auto mods) {
			((Window*)glfwGetWindowUserPointer(window))->input.mousePressedUpdate(button, action, mods);
		});

		glfwSetScrollCallback(window, [](auto window, auto xOffset, auto yOffset) {
			((Window*)glfwGetWindowUserPointer(window))->input.scrollUpdate(xOffset, yOffset);
		});
	}

	/// switches which monitor this window uses for fullscreen
	void Window::setMonitor(GLFWmonitor* monitor) {
		this->monitor = monitor;
	}

	/// makes the window either fullscreen or not
	/// with the stored internal monitor
	void Window::setFullscreen(bool full) {
		this->full = full;

		glfwSetWindowMonitor(window, full ? monitor : nullptr, x, y, width, height, GLFW_DONT_CARE);
	}

	/// collects glfw events
	/// put this at the startLoading of the game loop
	void Window::poll() {
		input.update();
		glfwPollEvents();
	}

	/// put this at the end of the game loop
	void Window::swap() {
		glfwSwapBuffers(window);
	}

	/// as soon as events are polled this window is going bye bye
	void Window::close() {
		glfwDestroyWindow(window);

		glfwTerminate();
	}

	auto Window::setIcon() -> void {
		auto img = GLFWimage();
		img.pixels = nullptr;
		
		glfwSetWindowIcon(window, 1, &img);
	}

	/*
	 * getters
	 */

		/// put thisd in a loop to know when the window is closed
	bool Window::getShouldClose() {
		return glfwWindowShouldClose(window);
	}

	int Window::getRefreshRate() {
		return videoMode->refreshRate;
	}

	int Window::getWidth() {
		return width;
	}

	int Window::getHeight() {
		return height;
	}

	Input* Window::getInput() {
		return &input;
	}
}
