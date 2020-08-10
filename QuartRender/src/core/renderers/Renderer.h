#pragma once
#include <vector>
#include <queue>
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../VertexArray.h"
#include "../IndexBuffer.h"
#include "../ShaderProgram.h"
#include "IRenderer.h"

//an offscreen renderer
class Renderer final: public IRenderer
{
private:	
	GLuint fbo;

	GLuint rbo_color, rbo_depth;
	std::vector<std::uint8_t> m_currentImageBuffer;

	bool m_isInit = false;


	void m_draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shaderProgram)const;

	void m_deleteGLBuffers();

public:
	//TODO: add default argument for clear color(default to 0,0,0,0)
	Renderer(unsigned int width, unsigned int height);
	~Renderer();

	bool windowShouldClose()override;

	void setViewportDimensions(unsigned int width, unsigned int height)override;

	void display()override;

	const std::vector<std::uint8_t>* getImageBuffer()const noexcept override;


};

