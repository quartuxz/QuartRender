#pragma once
#include <map>

#include "glm/glm.hpp"
#include "../../renderers/RendererThreadManager.h"


template<class drawableType>
class IDrawableFactory
{
public:
	typedef drawableType drawable_t;
protected:

	std::map<uniqueID, drawable_t*> m_testDrawablesPerRenderer;

	IDrawableFactory() {

	}

	virtual drawable_t* m_getNewDrawable() = 0;
public:
	//causes crashes if called using a destoyed renderer
	virtual void addDrawableToRenderer(RendererThreadManager* renderer, const typename drawable_t::variation_t& variation) {
		drawable_t* tempTestDrawable;
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

	virtual void destroyDrawableForRenderer(RendererThreadManager* renderer) {
		if (m_testDrawablesPerRenderer.contains(renderer->getID())) {
			renderer->executeOnThread([this, renderer]() {delete m_testDrawablesPerRenderer[renderer->getID()]; });
			m_testDrawablesPerRenderer.erase(renderer->getID());
		}
	}

	virtual ~IDrawableFactory() {

	}
};

