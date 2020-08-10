#pragma once
#include "IRenderer.h"
#include "IWindowedRenderer.h"

class OnScreenRendererIMGUI final : public IWindowedRenderer
{
public:
	OnScreenRendererIMGUI(unsigned int sizex, unsigned int sizey);

	void display()override;

	~OnScreenRendererIMGUI();
};

