#pragma once
#include <mutex>
#include <atomic>
#include <queue>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "inputStructs.h"


//used as a one-instance-per-window class
class InputManager
{
//TODO: implement input managers for each class instance
friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
friend void mouseButton_callback(GLFWwindow* window, int button, int action, int mods);
friend void cursorPosition_callback(GLFWwindow* window, double xpos, double ypos);
friend void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
friend class OnScreenRendererIMGUI;
private:
	//tells the input manager that all following key events are used by IMGUI
	void m_setIMGUIWantsKeyboard(bool value);

	InputManager()noexcept;
	std::atomic<bool> m_isValid;
	bool m_IMGUIWantsKeyboard = false;
	
	std::queue<KeyboardInput> m_keyInputInstances;
	CursorPosition m_currentCursorPosition = {0,0};
	std::queue<MouseButtonInput> m_mouseButtonInputInstances;
	std::queue<ScrollInput> m_scrollInputInstances;
public:
	//registers an input manager for a given thread, enabling it to call the Key_callback function.
	static void registerInputManagerInThread();
	//signals that the input manager for a thread is no longer usable.
	static void unregisterInputManagerInThread();
	//destroys all input manager instances, used and unused. Usually called at the end of the program.
	static void destroyAllInputManagers()noexcept;

	//gets the input manager for the calling thread.
	static InputManager* getInputManagerForThread();


	//returns the oldest keyboard input and pops it from the queue
	KeyboardInput getAndPopOldestKeyboardInput()noexcept;

	//returns the oldest mouse button input and pops it from the queue
	MouseButtonInput getAndPopOldestMouseButtonInput()noexcept;

	//returns the current cursor position
	CursorPosition getCurrentCursorPosition()const noexcept;

	//returns the oldest scroll input and pops it from the queue
	ScrollInput getAndPopOldestScrollInput()noexcept;

	bool isValid()noexcept;
};

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void mouseButton_callback(GLFWwindow* window, int button, int action, int mods);

void cursorPosition_callback(GLFWwindow* window, double xpos, double ypos);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);