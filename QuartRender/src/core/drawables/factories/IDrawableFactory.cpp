#include "IDrawableFactory.h"

IDrawableFactory::IDrawableFactory()
{
}

void IDrawableFactory::addDrawableToRenderer(RendererThreadManager* renderer, const DrawVariation& variation)
{
	IDrawable* tempTestDrawable;
	if (m_testDrawablesPerRenderer.contains(renderer->getID())) {
		tempTestDrawable = m_testDrawablesPerRenderer[renderer->getID()];

	}
	else {
		renderer->executeOnThread([&tempTestDrawable, this]() {tempTestDrawable = m_getNewDrawable(); });
		m_testDrawablesPerRenderer[renderer->getID()] = tempTestDrawable;
	}

	tempTestDrawable->addVariation(variation);




	renderer->addDrawable(tempTestDrawable);
}

void IDrawableFactory::destroyDrawableForRenderer(RendererThreadManager* renderer)
{
	if (m_testDrawablesPerRenderer.contains(renderer->getID())) {
		renderer->executeOnThread([this, renderer]() {delete m_testDrawablesPerRenderer[renderer->getID()]; });
		m_testDrawablesPerRenderer.erase(renderer->getID());
	}
}

IDrawableFactory::~IDrawableFactory()
{
}
