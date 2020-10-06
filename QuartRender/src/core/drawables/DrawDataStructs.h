#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



enum class DrawDataGetFlags :uint32_t {
	getZoomed = 1 << 0,
	getDisplacement = 1 << 1
};

enum class DrawDataGetDimensions {
	get3D,
	get2D
};


#define DEFAULT_DRAW_GET_FLAGS DrawDataGetFlags::getDisplacement,DrawDataGetFlags::getZoomed

/// <summary>
/// implements all the parameters for a "camara" of sorts
/// </summary>
struct DrawData {
private:

	typedef std::pair<glm::f64mat4, size_t> updateableMatrix;

	glm::f64mat4 m_view;
	glm::f64mat4 m_projection2D;
	glm::f64mat4 m_projection3D;


	glm::f64mat4 m_viewProj2D;
	glm::f64mat4 m_viewProj3D;


	double m_zoomLevel = 1;
	glm::f64vec3 m_displacement = glm::f64vec3(0,0,0);

	//unuseed things, their purpose was to facilitate pre-calculation caching,
	//but it would be exorbitant to implement now at least.
	updateableMatrix m_viewProj2DZoom;
	updateableMatrix m_viewProj3DZoom;


	void m_checkAndUpdateZoom();


	size_t m_zoomUpdate = 0;

public:


	DrawData(const glm::f64mat4& view, const glm::f64mat4& projection2D, const glm::f64mat4& projection3D);

	void setView(const glm::f64mat4 &view)noexcept;
	void setProjection2D(const glm::f64mat4 &projection2D)noexcept;
	void setProjection3D(const glm::f64mat4 &projection3D)noexcept;

	
	//void viewDisplacement(glm::vec2);
	//void getViewDisplacement();


	//all the zoom setters and getters:
		void zoom(double delta)noexcept;
		void setZoom(double value)noexcept;
		double getZoom()const noexcept;
	//~

	void displace(const glm::f64vec3& delta);
	glm::f64vec3 getDisplacement()const noexcept;

	/// <summary>
	/// gets only the projection matrix
	/// </summary>
	/// <param name="get3D">indicates wether it is the 2d or the 3d version you need</param>
	/// <returns></returns>
	glm::f64mat4 getProjection(DrawDataGetDimensions what2DOr3D)const noexcept;


	/// <summary>
	/// a more sane version of the templated getViewProj function
	/// calculates the viewProj with the given flags on the fly applied in the order specified.
	/// </summary>
	/// <param name="flags">the flags to apply, make sure not to repeat them</param>
	/// <returns></returns>
	glm::f64mat4 getViewProj(DrawDataGetDimensions what2DOr3D, const std::vector<DrawDataGetFlags> &flags)const noexcept;

	/// <summary>
	/// deprecated!
	/// this is probably a bad idea and I may just implement it if performance suffers.
	/// gets the view projection matrix with the given transformations applied
	/// this will grow to become a very complex function with
	/// 2^n + 2^(n-1) + 2^(n-2) + ... + 2^1 
	/// if + else statements where n is the number of unique flags
	/// </summary>
	/// <param name="flags"></param>
	/// <returns></returns>
	template<DrawDataGetDimensions what2DOr3D,uint32_t flags>
	glm::f64mat4 getViewProj()noexcept {
		if constexpr(what2DOr3D == DrawDataGetDimensions::get3D) {
			if constexpr((uint32_t(DrawDataGetFlags::getZoomed)&flags)>0) {
				if (m_viewProj3DZoom.second != m_zoomUpdate) {
					m_viewProj3DZoom.second = m_zoomUpdate;
				}
				return m_viewProj3DZoom.first;
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
