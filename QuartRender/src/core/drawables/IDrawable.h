#pragma once
#include <queue>
#include <optional>
#include "DrawDataStructs.h"
#include "glm/glm.hpp"

enum class DrawableTypes:unsigned int {
	drawable2D, drawable3D
};

class IDrawable
{
protected:
	glm::mat4 m_modelTransform;
	std::queue<glm::mat4> m_modelTransformQueue;
public:
	//TODO: make function take 1.) view and projection matrices
	//TODO: 2.)light sources
	virtual void draw(const DrawData&) = 0;
	//gets the type of drawable this is(e.g. 2D, 3D etc)
	virtual DrawableTypes getDrawableType() = 0;
	//set the required state for each variation by popping the variations and using them per this function call
	virtual bool setAndPop() = 0;


	virtual ~IDrawable();
};

