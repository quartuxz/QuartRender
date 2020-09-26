#pragma once
#include <vector>

#include <GL/glew.h>
#include "glm/glm.hpp"

#include "../../VertexArray.h"
#include "../../IndexBuffer.h"
#include "../../ShaderProgram.h"
#include "../IDrawable.h"

struct PlanetCharacteristics {
	double radius;
};

class PlanetDrawable final: 
	public IDrawable
{
private:
	PlanetCharacteristics m_characteristics;

	std::pair<std::vector<GLfloat>, std::vector<GLuint>> *m_outlineVerticesAndIndices;

	VertexArray m_outlineVertexArray;
	IndexBuffer m_outlineIndexBuffer;
	ShaderProgram m_outlineProgram;
	VertexBuffer m_outlineVertexBuffer;


	UniformGL<UniformTypes::FLOAT_4>* u_color;
	UniformGL<UniformTypes::FLOAT_MAT_4x4>* u_MVP;
public:
	PlanetDrawable(const PlanetCharacteristics &characteristics);
	bool setAndPop()override;
	DrawableTypes getDrawableType()const noexcept override;

	void draw(const DrawData& drawData)override;

	~PlanetDrawable();
};

