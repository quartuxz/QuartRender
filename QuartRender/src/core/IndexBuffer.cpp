#include "IndexBuffer.h"
#include "utilsGL.h"

IndexBuffer::IndexBuffer(const GLuint* data, unsigned int count):
    m_count(count)
{
    THROW_ERRORS_GL(glGenBuffers(1, &m_GLID));
    THROW_ERRORS_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GLID));
    THROW_ERRORS_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(GLuint), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    THROW_ERRORS_GL(glDeleteBuffers(1,&m_GLID));
}

void IndexBuffer::bind()const
{
    THROW_ERRORS_GL_FAST(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GLID));
}

void IndexBuffer::unbind()const
{
    THROW_ERRORS_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned int IndexBuffer::getCount() const
{
    return m_count;
}
