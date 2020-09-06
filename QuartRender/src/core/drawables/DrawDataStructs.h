#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


//implements all the parameters for a "camara" of sorts
struct DrawData {
private:
	glm::mat4 m_view;
	glm::mat4 m_projection2D;
	glm::mat4 m_projection3D;
	glm::mat4 m_viewProj2D;
	glm::mat4 m_viewProj3D;

public:


	DrawData(glm::mat4 view, glm::mat4 projection2D, glm::mat4 projection3D):
	m_view(view),
	m_projection2D(projection2D),
	m_projection3D(projection3D)
	{
		m_viewProj2D = m_projection2D * m_view;
		m_viewProj3D = m_projection3D * m_view;
	}

	void setView(glm::mat4 view)noexcept {
		m_view = view;
		m_viewProj2D = m_projection2D*m_view;
		m_viewProj3D = m_projection3D*m_view;
	}
	void setProjection2D(glm::mat4 projection2D)noexcept {
		m_projection2D = projection2D;
		m_viewProj2D = m_projection2D * m_view;
	}
	void setProjection3D(glm::mat4 projection3D)noexcept {
		m_projection3D = projection3D;
		m_viewProj3D = m_projection3D * m_view;
	}

	glm::mat4 getView()const noexcept {
		return m_view;
	}

	glm::mat4 getProjection2D()const noexcept {
		return m_projection2D;
	}

	glm::mat4 getProjection3D()const noexcept {
		return m_projection3D;
	}

	glm::mat4 getViewProj2D()const noexcept {
		return m_viewProj2D;
	}

	glm::mat4 getViewProj3D()const noexcept {
		return m_viewProj3D;
	}

};
