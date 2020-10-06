#pragma once
#include "../core/drawables/factories/IDrawableFactory.h"


class TestCubeDrawableFactory final: public IDrawableFactory
{
private:
	IDrawable* m_getNewDrawable()override;
public:
	TestCubeDrawableFactory();
};

