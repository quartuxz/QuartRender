#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



enum class DrawDataGetFlags :uint32_t {
	get3D = 1 << 0,
	get2D = 1 << 1,
	getZoomed = 1 << 2,
	getDisplacement = 1 << 3
};


#define DEFAULT_2D_GET_FLAGS DrawDataGetFlags::get2D,DrawDataGetFlags::getZoomed,DrawDataGetFlags::getDisplacement

//implements all the parameters for a "camara" of sorts
struct DrawData {
private:

	typedef std::pair<glm::mat4, size_t> updateableMatrix;

	glm::mat4 m_view;
	glm::mat4 m_projection2D;
	glm::mat4 m_projection3D;


	glm::mat4 m_viewProj2D;
	glm::mat4 m_viewProj3D;


	double m_zoomLevel = 1;
	glm::vec3 m_displacement = glm::vec3(0,0,0);

	//unuseed things, their purpose was to facilitate pre-calculation caching,
	//but it would be exorbitant to implement now at least.
	updateableMatrix m_viewProj2DZoom;
	updateableMatrix m_viewProj3DZoom;


	void m_checkAndUpdateZoom();


	size_t m_zoomUpdate = 0;

public:


	DrawData(glm::mat4 view, glm::mat4 projection2D, glm::mat4 projection3D);

	void setView(glm::mat4 view)noexcept;
	void setProjection2D(glm::mat4 projection2D)noexcept;
	void setProjection3D(glm::mat4 projection3D)noexcept;

	
	//void viewDisplacement(glm::vec2);
	//void getViewDisplacement();

	void zoom(double delta);
	double getZoom()const noexcept;

	void displace(glm::vec3 delta);
	glm::vec3 getDisplacement()const noexcept;

	/// <summary>
	/// gets only the projection matrix
	/// </summary>
	/// <param name="get3D">indicates wether it is the 2d or the 3d version you need</param>
	/// <returns></returns>
	glm::mat4 getProjection(bool get3D)const noexcept;


	/// <summary>
	/// a more sane version of the templated getViewProj function
	/// calculates the viewProj with the given flags on the fly applied in the order specified.
	/// </summary>
	/// <param name="flags">the flags to apply, make sure not to repeat them</param>
	/// <returns></returns>
	glm::mat4 getViewProj(const std::vector<DrawDataGetFlags> &flags)const noexcept;

	/// <summary>
	/// this is probably a bad idea and I may just implement it if performance suffers.
	/// gets the view projection matrix with the given transformations applied
	/// this will grow to become a very complex function with
	/// 2^n + 2^(n-1) + 2^(n-2) + ... + 2^1 
	/// if + else statements where n is the number of unique flags
	/// </summary>
	/// <param name="flags"></param>
	/// <returns></returns>
	template<uint32_t flags>
	glm::mat4 getViewProj()noexcept {
		if constexpr((uint32_t(DrawDataGetFlags::get3D) & flags) > 0) {
			if constexpr((uint32_t(DrawDataGetFlags::getZoomed)&flags)>0) {
				if (m_viewProj2DZoom.second != m_zoomUpdate) {
					m_viewProj2DZoom.second = m_zoomUpdate;
				}
				return m_viewProj2DZoom.first;
			}
			else {
				return m_viewProj3D;
			}
			
		}
		else{
			return m_viewProj2D;
		}
	}

};
