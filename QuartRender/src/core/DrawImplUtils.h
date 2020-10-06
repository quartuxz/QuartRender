#pragma once
#include "../core/drawables/IDrawable.h"
#include "../core/VertexArray.h"
#include "../core/IndexBuffer.h"
#include "../core/ShaderProgram.h"

#include "../core/drawables/IDrawable.h"


typedef std::pair<std::vector<GLfloat>, std::vector<GLuint>> IndicesAndVertices_t;

#define DEFAULT_FIELDS_FOR_DRAWABLE(name)	VertexArray m_##name##VertexArray; \
											IndexBuffer m_##name##IndexBuffer; \
											ShaderProgram m_##name##Program; \
											VertexBuffer m_##name##VertexBuffer;

