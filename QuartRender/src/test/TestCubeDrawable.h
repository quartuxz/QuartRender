#pragma once
#include "../core/drawables/IDrawable.h"
#include "../core/DrawImplUtils.h"

class TestCubeDrawable final:
	public IDrawable
{
private:
	IndicesAndVertices_t *m_cubeVerticesAndIndices;

	DEFAULT_FIELDS_FOR_DRAWABLE()

	u_MVP_t* u_MVP;
public:

	TestCubeDrawable();

	DrawableTypes getDrawableType()const noexcept override;

	void draw(const DrawData& drawData)override;
	
	~TestCubeDrawable();
};

