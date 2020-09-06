#pragma once
#include "../../vendor/imgui/imgui.h"
#include "../../vendor/imgui/imgui_impl_glfw.h"
#include "../../vendor/imgui/imgui_impl_opengl3.h"
#include "IRenderer.h"
#include "IWindowedRenderer.h"

class OnScreenRendererIMGUI final : public IWindowedRenderer
{
private:

	ImGuiIO* m_io;

	void m_startIMGUIFrame();
public:
	OnScreenRendererIMGUI(unsigned int sizex, unsigned int sizey);

	void display()override;

	~OnScreenRendererIMGUI();
};

