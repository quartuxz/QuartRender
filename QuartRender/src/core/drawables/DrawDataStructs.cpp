#include "DrawDataStructs.h"
#include <algorithm>

//unused
void DrawData::m_checkAndUpdateZoom()
{
}

DrawData::DrawData(const glm::f64mat4& view, const glm::f64mat4& projection2D, const glm::f64mat4& projection3D) :
	m_view(view),
	m_projection2D(projection2D),
	m_projection3D(projection3D)
{
	m_viewProj2D = m_projection2D * m_view;
	m_viewProj3D = m_projection3D * m_view;
}

void DrawData::setView(const glm::f64mat4& view) noexcept
{
	m_view = view;
	m_viewProj2D = m_projection2D * m_view;
	m_viewProj3D = m_projection3D * m_view;
}

void DrawData::setProjection2D(const glm::f64mat4& projection2D) noexcept {
	m_projection2D = projection2D;
	m_viewProj2D = m_projection2D * m_view;
}

void DrawData::setProjection3D(const glm::f64mat4& projection3D) noexcept
{
	m_projection3D = projection3D;
	m_viewProj3D = m_projection3D * m_view;
}

void DrawData::zoom(double delta)noexcept
{
	m_zoomLevel += delta;
	m_zoomLevel = std::max(m_zoomLevel, 0.0);
}

void DrawData::setZoom(double value)noexcept
{
	m_zoomLevel = value;
}

double DrawData::getZoom() const noexcept
{
	return m_zoomLevel;
}

void DrawData::displace(const glm::f64vec3& delta)
{
	m_displacement += delta;
}

glm::f64vec3 DrawData::getDisplacement() const noexcept
{
	return m_displacement;
}

glm::f64mat4 DrawData::getProjection(DrawDataGetDimensions what2DOr3D) const noexcept
{

	if (what2DOr3D == DrawDataGetDimensions::get2D) {
		return m_projection2D;
	}
	else {
		return m_projection3D;
	}
}

glm::f64mat4 DrawData::getViewProj(DrawDataGetDimensions what2DOr3D, const std::vector<DrawDataGetFlags>& flags)const noexcept
{
	glm::f64mat4 retval = m_view;


	//TODO: make 3d view transformations work down here
	for (auto x : flags) {
		switch (x) {
			case DrawDataGetFlags::getZoomed:
				if (what2DOr3D == DrawDataGetDimensions::get2D) {
					glm::f64mat4 zoomMat = glm::scale(glm::f64mat4(1.0), glm::f64vec3(m_zoomLevel, m_zoomLevel, 1));
					retval = zoomMat * retval;
				}
				else {
					glm::f64mat4 zoomMat = glm::scale(glm::f64mat4(1.0), glm::f64vec3(m_zoomLevel, m_zoomLevel, 1));
					retval = zoomMat * retval;
				}
				break;
			case DrawDataGetFlags::getDisplacement:
				if (what2DOr3D == DrawDataGetDimensions::get2D) {
					//copy assingment probably slower than direct multiplication with rvalue, I dont care for now
					//same happens in other places nearby
					glm::f64mat4 displacementMat = glm::translate(glm::f64mat4(1.0f), m_displacement);
					retval = displacementMat * retval;
				}
				else {
					glm::f64mat4 displacementMat = glm::translate(glm::f64mat4(1.0f), m_displacement);
					retval = displacementMat * retval;
				}
				
				break;
			default:
				break;
		}
	}

	if (what2DOr3D == DrawDataGetDimensions::get2D) {
		retval = m_projection2D * retval;
	}
	else {
		retval = m_projection3D * retval;

	}

	return retval;
}


