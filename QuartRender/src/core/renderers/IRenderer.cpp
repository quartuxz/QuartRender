#include "IRenderer.h"
#include "../utilsGL.h"


//TODO: consider moving the viewport resize capabilities here

void IRenderer::m_clear() const
{
	THROW_ERRORS_GL_FAST(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

IRenderer::IRenderer(unsigned int sizex, unsigned int sizey):
	m_width(sizex),
	m_height(sizey)
{
	m_vp2D = m_proj2D * m_view;
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
			if (m_drawQueue.front()->getDrawableType() == DrawableTypes::drawable2D) {
				m_drawQueue.front()->draw(m_vp2D);
			}//TODO: implement 3d drawable drawing
			else {

			}

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
	m_width = sizex;
	m_height = sizey;
}

void IRenderer::addDrawable(IDrawable* drawable)
{
	m_drawQueue.push(drawable);
}

IRenderer::~IRenderer()
{
	glfwDestroyWindow(m_window);

}
