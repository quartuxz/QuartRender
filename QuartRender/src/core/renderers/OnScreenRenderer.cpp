#include "OnScreenRenderer.h"
#include <stdexcept>

#include "../../vendor/imgui/imgui.h"
#include "../../vendor/imgui/imgui_impl_glfw.h"
#include "../../vendor/imgui/imgui_impl_opengl3.h"


#include "../InputManager.h"

#include "../utilsGL.h"


OnScreenRenderer::OnScreenRenderer(unsigned int sizex, unsigned int sizey):
	IWindowedRenderer(sizex,sizey)
{

}


OnScreenRenderer::~OnScreenRenderer()
{

}
