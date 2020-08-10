#pragma once
#include <GL/glew.h>

class IndexBuffer {
private:
	GLuint m_GLID;
	unsigned int m_count;
public:
	//takes 32-bit unsigned integer indices. count specifies the number of them(to calculate: sizeof(GLuint)*count)
	IndexBuffer(const GLuint* data, unsigned int count);
	~IndexBuffer();

	void bind()const;
	void unbind()const;

	unsigned int getCount()const;
};