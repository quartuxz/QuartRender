#pragma once
#include "IRenderer.h"
#include "IWindowedRenderer.h"

class OnScreenRenderer final: public IWindowedRenderer
{
public:
	OnScreenRenderer(unsigned int sizex, unsigned int sizey);

	~OnScreenRenderer();
};

