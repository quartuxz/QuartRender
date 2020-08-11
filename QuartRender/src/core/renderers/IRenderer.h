#pragma once
#include <vector> 
#include <queue>
#include <set>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "../generic utils/IUniquelyIdentifiable.h"
#include "../Drawables/IDrawable.h"

enum class RendererTypes:unsigned int {
	onscreenRenderer, onscreenRendererIMGUI, offscreenRenderer, ENUM_MAX
};

class IRenderer
{
protected:
	GLFWwindow* m_window;
	std::queue<IDrawable*> m_drawQueue;

	glm::mat4 m_proj2D = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	glm::mat4 m_view = glm::mat4(1.0f);

	glm::mat4 m_vp2D;


	unsigned int m_width, m_height;

	virtual void m_clear()const;


	IRenderer(unsigned int sizex, unsigned int sizey);
public:



	virtual bool windowShouldClose();

	virtual void display();
	virtual const std::vector<std::uint8_t>* getImageBuffer()const noexcept;


	virtual unsigned int getViewportWidth()const noexcept;
	virtual unsigned int getViewportHeight()const noexcept;

	//TODO: test what happens when I put noexcept here
	virtual void setViewportDimensions(unsigned int sizex, unsigned int sizey);

	virtual void addDrawable(IDrawable *drawable);

	virtual ~IRenderer();

};
