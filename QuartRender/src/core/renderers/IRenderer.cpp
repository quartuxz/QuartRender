#include "IRenderer.h"
#include "../utilsGL.h"


bool IRenderer::m_glfwIsInit = false;

//TODO: consider moving the viewport resize capabilities here

void IRenderer::m_clear() const
{
	THROW_ERRORS_GL_FAST(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

IRenderer::IRenderer(unsigned int sizex, unsigned int sizey):
	m_width(sizex),
	m_height(sizey),
	//TODO: actually implement the projection properly(second argument to this function is perspective projection matrix)
	m_drawData(glm::mat4(1.0f), glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f), glm::mat4(1.0f))
{
	if (!m_glfwIsInit) {
		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit())
			throw std::runtime_error("failed to initialize glfw!");
		m_glfwIsInit = true;
	}
}

bool IRenderer::windowShouldClose()
{
	return glfwWindowShouldClose(m_window);
}

//TODO: implement 3d projection matrix and integrate with IDrawable to distinguish which to use
void IRenderer::display()
{

	LOG_TO_CONSOLE_COND("display started.");
	m_clear();
	LOG_TO_CONSOLE_COND("screen cleared.");
	while (!m_drawQueue.empty()) {
		if (m_drawQueue.front()->setAndPop()) {
			m_drawQueue.front()->draw(m_drawData);
			/*//old code no longer needed as we dont discriminate who gets sent what drawData we just let
			//the drawables choose
			if (m_drawQueue.front()->getDrawableType() == DrawableTypes::drawable2D ) {
				
			}//TODO: implement 3d drawable drawing
			else {

			}
			*/

		}		
		m_drawQueue.pop();
	}
	LOG_TO_CONSOLE_COND("draw queue finished.");
}

const std::vector<std::uint8_t>* IRenderer::getImageBuffer() const noexcept
{
	return nullptr;
}

unsigned int IRenderer::getViewportWidth() const noexcept
{
	return m_width;
}

unsigned int IRenderer::getViewportHeight() const noexcept
{
	return m_height;
}

void IRenderer::setViewportDimensions(unsigned int sizex, unsigned int sizey)
{
	//TODO: actually implement the projection properly(second argument to this function is perspective projection matrix)
	//the line below sets a new ortho-projection matrix(only 2d) that scales the viewport to not allow it to be squished
	//this works for both window-bound and to-memory rendering routines, its only purpose is to scale all geometry to always be portrayed
	//correctly regardless of the dimensions of the viewport.
	m_drawData.setProjection2D(glm::ortho(-(sizex/(float)sizey),(sizex/(float)sizey),-1.0f,1.0f,-1.0f,1.0f));
	m_width = sizex;
	m_height = sizey;
}

void IRenderer::addDrawable(IDrawable* drawable)
{
	m_drawQueue.push(drawable);
}

DrawData& IRenderer::getDrawDataRef() noexcept
{
	return m_drawData;
}

IRenderer::~IRenderer()
{

	
}
