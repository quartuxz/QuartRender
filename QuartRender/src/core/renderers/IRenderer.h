#pragma once
#include <vector> 
#include <queue>
#include <set>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GL/glew.h"
#include <GLFW/glfw3.h>


#include "../drawables/DrawDataStructs.h"
#include "../generic utils/IUniquelyIdentifiable.h"
#include "../Drawables/IDrawable.h"

enum class RendererTypes:unsigned int {
	onscreenRenderer, onscreenRendererIMGUI, offscreenRenderer, ENUM_MAX
};

//TODO: add camara(zoom, pan etc) and reconfigurable viewport capabilities to calculate the vp matrices
class IRenderer
{
protected:

	//move these to some settings singleton class
	//TODO: this was 45
	float m_FOVInDegrees = 45.0f;
	float m_zNear = 0.1f;
	float m_zFar = 10000.0f;

	GLFWwindow* m_window = nullptr;
	std::queue<IDrawable*> m_drawQueue;

	DrawData m_drawData;


	static bool m_glfwIsInit;

	unsigned int m_width, m_height;


	virtual void m_clear()const;


	IRenderer(unsigned int sizex, unsigned int sizey);
public:

	IRenderer& operator=(IRenderer)= delete;
	IRenderer(const IRenderer&) = delete;

	virtual bool windowShouldClose();

	virtual void display();
	virtual const std::vector<std::uint8_t>* getImageBuffer()const noexcept;


	unsigned int getViewportWidth()const noexcept;
	unsigned int getViewportHeight()const noexcept;

	//TODO: test what happens when I put noexcept here
	virtual void setViewportDimensions(unsigned int sizex, unsigned int sizey);

	virtual void addDrawable(IDrawable *drawable);

	DrawData& getDrawDataRef()noexcept;
	const DrawData& getDrawDataRef()const noexcept;

	virtual ~IRenderer();

};

