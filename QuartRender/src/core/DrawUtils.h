#pragma once
#include <vector>

#include <GL/glew.h>
#include "glm/glm.hpp"

/*//me trying to translate model coordinates to global and then camara coordinates cpu side
//bad idea i think.
inline std::vector<double> applyMatrixToVertices2D(const std::vector<double>& vertices, const glm::f64mat4 &mat) {
	typedef decltype(applyMatrixToVertices2D({}, glm::f64mat4(0))) rettype;
	rettype retval;
	retval.reserve(vertices.size());
	for (size_t i = 0; i < vertices.size(); i+=2) {
		glm::f64vec4 applied = mat * glm::f64vec4(vertices[i], vertices[i + 1], 1.0f, 1.0f);


	}
}
*/

