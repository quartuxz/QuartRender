#include "VertexBuffer.h"
#include "utilsGL.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    THROW_ERRORS_GL(glGenBuffers(1, &m_GLID));
    bind();
    THROW_ERRORS_GL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    THROW_ERRORS_GL(glDeleteBuffers(1,&m_GLID));
}

void VertexBuffer::bind()const
{
    THROW_ERRORS_GL(glBindBuffer(GL_ARRAY_BUFFER, m_GLID));
}

void VertexBuffer::unbind()const
{
    THROW_ERRORS_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
