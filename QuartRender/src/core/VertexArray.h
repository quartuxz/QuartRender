#pragma once
#include <GL/glew.h>
#include <vector>
#include <map>
#include "VertexBuffer.h"



class BufferLayoutElement {
friend class BufferLayout;
friend class CustomBufferLayout;
friend class IBufferLayout;
private:
	GLboolean m_shouldNormalize;
	GLuint m_numberOfElements;
	GLenum m_type;
	GLuint m_attributeIndex = 0;
	GLuint m_attributeOffset = 0;

	size_t m_sizeInBytes;

	//static CustomPerVertexLayout sm_createCommonLayout(CommonPerVertexLayouts commonLayout, GLsizei* totalSize);
	void m_bindLayout(GLsizei stride)const;


public:

	BufferLayoutElement(GLuint numberOfElements, GLenum type, GLboolean shouldNormalize)noexcept;
	
};

class IBufferLayout {
friend class VertexArray;
protected:

	std::vector<BufferLayoutElement> m_layouts;
	GLsizei m_layoutSize;
	IBufferLayout()noexcept;
	IBufferLayout(GLsizei layoutSize)noexcept;

	void m_bindLayout()const;

public:
	virtual GLsizei getLayoutSize()const noexcept;



	virtual ~IBufferLayout()noexcept;
};



//use this if your vertices are an array of structs(together with attributeOffset parameter+the offsetoff macro) to make sure padding is compatible with layout
class CustomBufferLayout:public IBufferLayout {
public:
	//takes the entire per vertex size
	CustomBufferLayout(GLsizei layoutSize)noexcept;

	void addBufferLayoutElement(GLuint attributeIndex, GLuint numberOfElements, GLenum type, GLboolean shouldNormalize, GLuint attributeOffset);
};


class BufferLayout:public IBufferLayout {
private:
	GLuint m_currentOffset = 0;
public:
	//TODO: create more common buffer layouts
	enum CommonBufferLayouts {
		LAYOUT_2D_POSITION_ONLY,
		LAYOUT_3D_POSITION_ONLY,
		LAYOUT_2D_POSITION_4COLOR,
		LAYOUT_3D_POSITION_4COLOR
	};
	//TODO: implement this
	//creates a buffer layout with common caracteristics decribed in CommonBufferLayouts.
	BufferLayout(CommonBufferLayouts commonBufferLayout);

	void addBufferLayoutElement(BufferLayoutElement bufferLayoutElement);
	BufferLayout()noexcept;
};


class VertexArray
{
private:
	std::vector<const IBufferLayout*> m_BufferLayouts;

	GLuint m_GLID;

	void m_addLayout(const VertexBuffer* vertexBuffer,const IBufferLayout& bufferLayout);
public:
	VertexArray();
	//TODO: add checking for if two of these function calls add the same buffer
	//(ideally only one function call per buffer is needed given a different layout than when two are used)
	void addBuffer(const VertexBuffer* vertexBuffer, const IBufferLayout& bufferLayout);


	void bind()const;
	void unbind()const;

	~VertexArray();

};

