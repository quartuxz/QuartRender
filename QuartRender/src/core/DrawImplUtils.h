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

/// <summary>
/// adds three elements contained in a glm::vec3 to a given vector.
/// </summary>
/// <param name="addTo"></param>
/// <param name="added"></param>
/// <param name="extraReserve">IMPORTANT: this specifies how much space to reserve+the current capacity
/// default is 3; set to 0 if you have reserved space beforehand</param>
inline void addVecToVector(std::vector<GLfloat> &addTo, const glm::vec3 &added, size_t extraReserve = 3) {
	//worth noting that reserving capacity+0 is a no-op by an if check inside such function
	//(atleast in visual studio 2019 compiler)
	addTo.reserve(addTo.capacity()+extraReserve);
	addTo.push_back(added.x);
	addTo.push_back(added.y);
	addTo.push_back(added.z);
}

