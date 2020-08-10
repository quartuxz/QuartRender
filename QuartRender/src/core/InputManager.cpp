#include "InputManager.h"
#include <thread>
#include <map>
#include <vector>

static std::map<std::thread::id, InputManager*> inputManagers;
static std::vector<InputManager*> unregisteredInputManagers;

#define THIS_IMANAGER inputManagers[std::this_thread::get_id()]

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto threadid = std::this_thread::get_id();
}

InputManager::InputManager()noexcept
{
	m_windowIsOpen.store(true);
}

void InputManager::registerInputManagerInThread()
{
	THIS_IMANAGER = new InputManager();
}

void InputManager::unregisterInputManagerInThread()
{
	THIS_IMANAGER->m_windowIsOpen.store(false);
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

bool InputManager::isWindowOpen()noexcept
{
	return m_windowIsOpen.load();
}
