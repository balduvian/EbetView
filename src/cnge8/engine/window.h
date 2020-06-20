
#ifndef CNGE_WINDOW
#define CNGE_WINDOW

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <functional>

#include "input.h"

namespace CNGE {
	class Window {
	public:
		/// initializes GLFW
		/// you must call this before any other functions
		static void init();

		/// throws if glew does not initialize
		static void wrangle();

		/// returns an array of pointers to each monitor
		/// the monitor count is passed in and will be sent out
		static GLFWmonitor** getMonitors(int&);
		/// returns the primary monitor for simple monitor getting
		static GLFWmonitor* getPrimaryMonitor();

		/// creates the window
		/// call more methods after this to modify window
		Window(
			int majorVersion,
			int minorVersion,
			bool resizable,
			bool decorated,
			const char* title,
			GLFWmonitor* monitor,
			bool full,
			bool vsync
		);

		/// switches which monitor this window uses for fullscreen
		void setMonitor(GLFWmonitor* monitor);

		/// makes the window either fullscreen or not
		/// with the stored internal monitor
		void setFullscreen(bool full);

		/// collects glfw events
		/// put this at the startLoading of the game loop
		void poll();

		/// put this at the end of the game loop
		void swap();

		/// as soon as events are polled this window is going bye bye
		void close();

		auto setIcon() -> void;
		
		/*
		 * getters
		 */

		/// put this in a loop to know when the window is closed
		bool getShouldClose();

		int getRefreshRate();

		int getWidth();
		int getHeight();

		Input* getInput();

	private:
		GLFWwindow* window;
		GLFWmonitor* monitor;
		const GLFWvidmode* videoMode;

		Input input;

		int x, y, width, height;
		bool full;
	};
}

#endif
