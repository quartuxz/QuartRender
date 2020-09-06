#pragma once
#include <mutex>
#include <atomic>
#include <queue>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "inputStructs.h"

class InputManager
{
//TODO: implement input managers for each class instance
friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
private:
	InputManager()noexcept;
	std::atomic<bool> m_isValid;
	bool m_IMGUIWantsKeyboard = false;
	std::queue<KeyboardInput> m_keyInputInstances;
public:
	//registers an input manager for a given thread, enabling it to call the Key_callback function.
	static void registerInputManagerInThread();
	//signals that the input manager for a thread is no longer usable.
	static void unregisterInputManagerInThread();
	//destroys all input manager instances, used and unused. Usually called at the end of the program.
	static void destroyAllInputManagers()noexcept;

	//gets the input manager for the calling thread.
	static InputManager* getInputManagerForThread();


	void setIMGUIWantsKeyboard(bool value);

	//returns the oldest keyboard input and pops it from the queue
	KeyboardInput getAndPopOldestInput()noexcept;

	bool isValid()noexcept;
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);