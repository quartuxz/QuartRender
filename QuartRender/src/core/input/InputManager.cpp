#include "InputManager.h"
#include "../utilsGL.h"
#include <thread>
#include <map>
#include <vector>

//these are per-thread instances that are associated with unqiue renderers
static std::map<std::thread::id, InputManager*> inputManagers;
static std::map<std::thread::id, IWindowedRenderer*> windowedRenderers;


static std::vector<InputManager*> unregisteredInputManagers;

//gets the input manager for this thread, which corresponds to a unique renderer
#define THIS_IMANAGER inputManagers[std::this_thread::get_id()]

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	CursorPosition currentCursorPos = THIS_IMANAGER->m_currentCursorPosition;
	currentCursorPos.capturedByIMGUI = THIS_IMANAGER->m_IMGUIWantsMouse;
	THIS_IMANAGER->m_scrollInputInstances.push(ScrollInput{xoffset,yoffset,currentCursorPos,true});
}

void mouseButton_callback(GLFWwindow* window, int button, int action, int mods)
{
	CursorPosition currentCursorPos = THIS_IMANAGER->m_currentCursorPosition;
	currentCursorPos.capturedByIMGUI = THIS_IMANAGER->m_IMGUIWantsMouse;
	THIS_IMANAGER->m_mouseButtonInputInstances.push(MouseButtonInput{ button,action,mods,currentCursorPos,true});
}

void cursorPosition_callback(GLFWwindow* window, double xpos, double ypos)
{
	THIS_IMANAGER->m_currentCursorPosition = CursorPosition{xpos, ypos, THIS_IMANAGER->m_IMGUIWantsMouse};
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	THIS_IMANAGER->m_keyInputInstances.push(KeyboardInput{key,scancode,action,mods, THIS_IMANAGER->m_IMGUIWantsKeyboard, true});
}

void windowResize_callback(GLFWwindow* window, int newWindowWidth, int newWindowHeight)
{
	windowedRenderers[std::this_thread::get_id()]->setViewportDimensions(newWindowWidth,newWindowHeight);
}

InputManager::InputManager()noexcept
{
	m_isValid.store(true);
}

void InputManager::registerInputManagerInThread(IWindowedRenderer *renderer)
{
	THIS_IMANAGER = new InputManager();
	windowedRenderers[std::this_thread::get_id()] = renderer;
}

void InputManager::unregisterInputManagerInThread()
{
	THIS_IMANAGER->m_isValid.store(false);
	unregisteredInputManagers.push_back(THIS_IMANAGER);
	inputManagers.erase(std::this_thread::get_id());
	windowedRenderers.erase(std::this_thread::get_id());
}

void InputManager::destroyAllInputManagers()noexcept
{
	for (auto inputManager : inputManagers) {
		delete inputManager.second;
	}

	for (auto inputManager:unregisteredInputManagers) {
		delete inputManager;
	}
}

InputManager* InputManager::getInputManagerForThread()
{
	return THIS_IMANAGER;
}

void InputManager::m_setIMGUIWantsKeyboard(bool value)
{
	m_IMGUIWantsKeyboard = value;
}

void InputManager::m_setIMGUIWantsMouse(bool value)
{
	m_IMGUIWantsMouse = value;
}


KeyboardInput InputManager::getAndPopOldestKeyboardInput()noexcept
{
	KeyboardInput retval;
	if (m_keyInputInstances.empty()) {
		retval.isValid = false;
	}
	else {
		retval = m_keyInputInstances.front();
		m_keyInputInstances.pop();
	}

	return retval;
}

MouseButtonInput InputManager::getAndPopOldestMouseButtonInput() noexcept
{

	MouseButtonInput retval;
	if (m_mouseButtonInputInstances.empty()) {
		retval.isValid = false;
	}
	else {
		retval = m_mouseButtonInputInstances.front();
		m_mouseButtonInputInstances.pop();
	}
	return retval;
}

CursorPosition InputManager::getCurrentCursorPosition()const noexcept
{
	return m_currentCursorPosition;
}

ScrollInput InputManager::getAndPopOldestScrollInput() noexcept
{
	ScrollInput retval;
	if (m_scrollInputInstances.empty()) {
		retval.isValid = false;
	}
	else {
		retval = m_scrollInputInstances.front();
		m_scrollInputInstances.pop();
	}
	return retval;
}

bool InputManager::isValid()noexcept
{
	return m_isValid.load();
}
