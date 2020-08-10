#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <mutex>
#include <atomic>

class InputManager
{
private:
	InputManager()noexcept;
	std::atomic<bool> m_windowIsOpen;
public:
	static void registerInputManagerInThread();
	static void unregisterInputManagerInThread();
	static void destroyAllInputManagers()noexcept;

	static InputManager* getInputManagerForThread();

	bool isWindowOpen()noexcept;
};

//TODO: implement input managers for each class instance
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);