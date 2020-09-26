#pragma once
#include <map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "TestDrawable.h"
#include "../core/renderers/RendererThreadManager.h"
#include "../core/drawables/factories/IDrawableFactory.h"

class TestDrawableFactory final: public IDrawableFactory
{
private:
	TestDrawable *m_getNewDrawable()override;
public:
	TestDrawableFactory();

	~TestDrawableFactory();
};

