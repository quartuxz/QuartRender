#include "InputManager.h"
#include "../utilsGL.h"
#include <thread>
#include <map>
#include <vector>

static std::map<std::thread::id, InputManager*> inputManagers;
static std::vector<InputManager*> unregisteredInputManagers;

#define THIS_IMANAGER inputManagers[std::this_thread::get_id()]

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	THIS_IMANAGER->m_keyInputInstances.push(KeyboardInput{key,scancode,action,mods, THIS_IMANAGER->m_IMGUIWantsKeyboard, true});
}

InputManager::InputManager()noexcept
{
	m_isValid.store(true);
}

void InputManager::registerInputManagerInThread()
{
	THIS_IMANAGER = new InputManager();
}

void InputManager::unregisterInputManagerInThread()
{
	THIS_IMANAGER->m_isValid.store(false);
	unregisteredInputManagers.push_back(THIS_IMANAGER);
	inputManagers.erase(std::this_thread::get_id());
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

void InputManager::setIMGUIWantsKeyboard(bool value)
{
	m_IMGUIWantsKeyboard = value;
}


KeyboardInput InputManager::getAndPopOldestInput()noexcept
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

bool InputManager::isValid()noexcept
{
	return m_isValid.load();
}