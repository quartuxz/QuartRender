#pragma once
#include <vector>

#include <GL/glew.h>
#include "glm/glm.hpp"


#include "../../DrawImplUtils.h"
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

	double m_screenSizeForPlanetOutline;



	IndicesAndVertices_t * m_outlineVerticesAndIndices;
	IndicesAndVertices_t* m_planetSphereVerticesAndIndices;
	IndicesAndVertices_t* m_orbitLineVerticesAndIndicies;

	VertexArray m_outlineVertexArray;
	IndexBuffer m_outlineIndexBuffer;
	ShaderProgram m_outlineProgram;
	VertexBuffer m_outlineVertexBuffer;

	DEFAULT_FIELDS_FOR_DRAWABLE(planetSphere)
	DEFAULT_FIELDS_FOR_DRAWABLE(orbitLine)

	//for the outline
	UniformGL<UniformTypes::FLOAT_4>* u_color;
	u_MVP_t* u_MVP;


	//for the sphere
	u_MVP_t *u_3DMVP;
	u_MVP_t *u_model;

	//for the orbit
	UniformGL<UniformTypes::FLOAT_4> *u_orbitLineColor;
	u_MVP_t* u_orbitLineMVP;

public:
	PlanetDrawable(const PlanetCharacteristics &characteristics);
	bool setAndPop()override;
	DrawableTypes getDrawableType()const noexcept override;

	void draw(const DrawData& drawData)override;

	~PlanetDrawable();
};

