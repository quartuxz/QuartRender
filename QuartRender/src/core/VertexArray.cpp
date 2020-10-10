#include "VertexArray.h"
#include "utilsGL.h"



void VertexArray::m_addLayout(const VertexBuffer* vertexBuffer, const IBufferLayout& bufferLayout)
{
	bind();
	vertexBuffer->bind();
	m_BufferLayouts.push_back(new IBufferLayout(bufferLayout));
	m_BufferLayouts.back()->m_bindLayout();
}

VertexArray::VertexArray()
{
	THROW_ERRORS_GL(glGenVertexArrays(1, &m_GLID));
}


//TODO: add checking for if two of these function calls add the same buffer
//(ideally only one function call per buffer is needed given a different layout than when two are used)
void VertexArray::addBuffer(const VertexBuffer* buffer, const IBufferLayout &bufferLayout)
{

	m_addLayout(buffer, bufferLayout);
	
}

void VertexArray::bind() const
{
	THROW_ERRORS_GL_FAST(glBindVertexArray(m_GLID));
}

void VertexArray::unbind() const
{
	THROW_ERRORS_GL_FAST(glBindVertexArray(0));
}

VertexArray::~VertexArray()
{
	for (auto bufferLayout:m_BufferLayouts) {
		delete bufferLayout;
	}
	THROW_ERRORS_GL(glDeleteVertexArrays(1, &m_GLID));
}



void BufferLayoutElement::m_bindLayout(GLsizei stride)const
{

	THROW_ERRORS_GL(glVertexAttribPointer(m_attributeIndex, m_numberOfElements, m_type, m_shouldNormalize, stride, (GLvoid*)(m_attributeOffset)));
	THROW_ERRORS_GL(glEnableVertexAttribArray(m_attributeIndex));
}


BufferLayoutElement::BufferLayoutElement(GLuint numberOfElements, GLenum type, GLboolean shouldNormalize)noexcept:
	m_numberOfElements(numberOfElements),
	m_type(type),
	m_shouldNormalize(shouldNormalize),
	m_sizeInBytes(getSizeOfGLType(type)*numberOfElements)
{
}

void CustomBufferLayout::addBufferLayoutElement(GLuint attributeIndex, GLuint numberOfElements, GLenum type, GLboolean isNormalized, GLuint attributeOffset)
{
	BufferLayoutElement bufferLayoutElement(numberOfElements,type,isNormalized);
	bufferLayoutElement.m_attributeIndex = attributeIndex;
	bufferLayoutElement.m_attributeOffset = attributeOffset;
	m_layouts.push_back(bufferLayoutElement);
}

CustomBufferLayout::CustomBufferLayout(GLsizei layoutSize)noexcept:
	IBufferLayout(layoutSize)
{

}

IBufferLayout::IBufferLayout()noexcept
{
	
}

IBufferLayout::IBufferLayout(GLsizei layoutSize)noexcept:
	m_layoutSize(layoutSize)
{
}

void IBufferLayout::m_bindLayout()const
{
	for (const BufferLayoutElement& bufferLayoutElement : m_layouts) {
		bufferLayoutElement.m_bindLayout(getLayoutSize());
	}
}

GLsizei IBufferLayout::getLayoutSize() const noexcept
{
	return m_layoutSize;
}

IBufferLayout::~IBufferLayout()
{
}

BufferLayout::BufferLayout(CommonBufferLayouts commonBufferLayout)
{
}

void BufferLayout::addBufferLayoutElement(BufferLayoutElement bufferLayoutElement)
{
	bufferLayoutElement.m_attributeIndex = (GLuint)m_layouts.size();
	bufferLayoutElement.m_attributeOffset = m_currentOffset;
	m_layoutSize += (GLuint)bufferLayoutElement.m_sizeInBytes;
	m_currentOffset += (GLuint)bufferLayoutElement.m_sizeInBytes;
	m_layouts.push_back(bufferLayoutElement);
	
}

BufferLayout::BufferLayout()noexcept
{
}
