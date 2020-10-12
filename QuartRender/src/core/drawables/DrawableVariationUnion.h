#pragma once
#include "glm/glm.hpp"


union DrawVariation {
	glm::f64mat4 modelTransform;

	struct {
		glm::f64mat4 modelTransform2D;
		glm::f64mat4 modelTransform3D;
	}modelTransform2D3DHybrid;
};