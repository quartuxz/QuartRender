#include "DrawableFactoryManager.h"

void DrawableFactoryManager::endFrame()
{
}

void DrawableFactoryManager::addDrawableFactory(std::string name, IDrawableFactory* factory)
{
	m_drawableFactories[name] = factory;
}

void DrawableFactoryManager::addClickableDrawableFactory(std::string name, IClickableDrawableFactory* factory)
{
	m_drawableFactories[name] = factory;
	m_clickableDrawableFactories[name] = factory;
}

void DrawableFactoryManager::drawAnonymous(RendererThreadManager* renderer, std::string factoryName, const DrawVariation& variation)
{
	m_drawableFactories.at(factoryName)->addDrawableToRenderer(renderer, variation);
}

void DrawableFactoryManager::draw(RendererThreadManager* renderer, std::string factoryName, std::string drawInstanceName, const DrawVariation& variation)
{
	m_clickableDrawableFactories.at(factoryName)->addDrawableToRenderer(renderer, variation, drawInstanceName);
}



void DrawableFactoryManager::destroyDrawablesForRenderer(RendererThreadManager* renderer)
{
	for (auto x : m_drawableFactories) {
		x.second->destroyDrawableForRenderer(renderer);
	}
}

DrawableFactoryManager::~DrawableFactoryManager()
{
	for (auto x : m_drawableFactories) {
		delete x.second;
	}
}
