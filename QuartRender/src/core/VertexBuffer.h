#pragma once
#include <GL/glew.h>
#include <vector>
#include "generic utils/IUniquelyIdentifiable.h"

class VertexBuffer: public IUniquelyIdentifiable {
private:
	GLuint m_GLID;
public:
	VertexBuffer(const void* data, unsigned int size);
	//loading a whole vertex-filled vector into the buffer
	VertexBuffer(const std::vector<GLfloat> &vertices);
	~VertexBuffer();

	VertexBuffer(const VertexBuffer&)= delete;
	VertexBuffer operator=(const VertexBuffer&)=delete;

	void bind()const;
	void unbind()const;
};