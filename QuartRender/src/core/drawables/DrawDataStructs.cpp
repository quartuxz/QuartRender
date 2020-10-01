#include "DrawDataStructs.h"
#include <algorithm>

//unused
void DrawData::m_checkAndUpdateZoom()
{
}

DrawData::DrawData(glm::mat4 view, glm::mat4 projection2D, glm::mat4 projection3D) :
	m_view(view),
	m_projection2D(projection2D),
	m_projection3D(projection3D)
{
	m_viewProj2D = m_projection2D * m_view;
	m_viewProj3D = m_projection3D * m_view;
}

void DrawData::setView(glm::mat4 view) noexcept
{
	m_view = view;
	m_viewProj2D = m_projection2D * m_view;
	m_viewProj3D = m_projection3D * m_view;
}

void DrawData::setProjection2D(glm::mat4 projection2D) noexcept {
	m_projection2D = projection2D;
	m_viewProj2D = m_projection2D * m_view;
}

void DrawData::setProjection3D(glm::mat4 projection3D) noexcept
{
	m_projection3D = projection3D;
	m_viewProj3D = m_projection3D * m_view;
}

void DrawData::zoom(double delta)
{
	m_zoomLevel += delta;
	m_zoomLevel = std::max(m_zoomLevel, 0.0);
}

double DrawData::getZoom() const noexcept
{
	return m_zoomLevel;
}

void DrawData::displace(glm::vec3 delta)
{
	m_displacement += delta;
}

glm::vec3 DrawData::getDisplacement() const noexcept
{
	return m_displacement;
}

glm::mat4 DrawData::getProjection(bool get3D) const noexcept
{

	if (get3D) {
		return m_projection3D;
	}
	else {
		return m_projection2D;
	}
}

glm::mat4 DrawData::getViewProj(const std::vector<DrawDataGetFlags>& flags)const noexcept
{
	glm::mat4 retval = glm::mat4(1.0f);

	for (auto x : flags) {
		switch (x) {
			case DrawDataGetFlags::get3D:
				retval = m_viewProj3D *retval;
				break;
			case DrawDataGetFlags::get2D:
				retval = m_viewProj2D * retval;
				break;
			case DrawDataGetFlags::getZoomed:
				glm::mat4 zoomMat = glm::scale(glm::mat4(1.0f), glm::vec3(m_zoomLevel,m_zoomLevel , 1));
				retval = zoomMat * retval;
				break;
			case DrawDataGetFlags::getDisplacement:
				glm::mat4 displacementMat = glm::translate(glm::mat4(1.0f), m_displacement);
				retval = displacementMat * retval;
				break;
			default:
				break;
		}
	}

	return retval;
}


