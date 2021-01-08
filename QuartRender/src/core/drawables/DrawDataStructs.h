#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



enum class DrawDataGetFlags :uint32_t {
	getZoomed = 1 << 0,
	getDisplacement = 1 << 1,
	//this one is different from normal zoom as it actually only scales a fixed Z distance to apply 
	//a z translation based on it
	getZPlaneZoomed = 1 << 2,
	//only works for 3d
	getFOVZoomed = 1 << 3
};

enum class DrawDataGetDimensions {
	get3D,
	get2D
};


//we apply displacement first so that the scaling(zoom) is applied about the centre of the view
//and not the absolute centre of the coordinate space; atleast for 2d.
#define DEFAULT_DRAW_GET_FLAGS DrawDataGetFlags::getDisplacement,DrawDataGetFlags::getZoomed
#define ZPLANE_ZOOMED_DRAW_GET_FLAGS DrawDataGetFlags::getZPlaneZoomed, DrawDataGetFlags::getDisplacement

//NOTE: all angles in radians for this struct
struct PerspectiveProjection {
private:
	float m_FOVY, m_aspect, m_near, m_far;
	glm::f64mat4 m_mat;
public:
	//FOVY in radians!
	PerspectiveProjection(float FOVY, float aspect, float near, float far);
	//constructs a perspective projection with everything similar except the FOVY which is specified.
	PerspectiveProjection(float FOVY, const PerspectiveProjection &other);
	float getFOVY()const noexcept;
	float getFOVX()const noexcept;
	float getAspect()const noexcept;
	float getNear()const noexcept;
	float getFar()const noexcept;
	const glm::f64mat4& getMatrix()const noexcept;

};


/// <summary>
/// implements all the parameters for a "camara" of sorts
/// </summary>
struct DrawData {
private:

	//TODO: do some matrix caching if things slow down.


	typedef std::pair<glm::f64mat4, size_t> updateableMatrix;

	glm::f64mat4 m_view;
	glm::f64mat4 m_projection2D;
	PerspectiveProjection m_projection3D;

	//AKA magnification factor
	double m_zoomLevel = 0.000001;

	//TODO: move this to a specific settings singleton class
	//for program-wide settings
	double m_initial3DZDisplacement = 1.0;
	
	glm::f64vec3 m_displacement = glm::f64vec3(0,0,0);

	//unused things, their purpose was to facilitate pre-calculation caching,
	//but it would be exorbitant to implement now at least.
	updateableMatrix m_viewProj2DZoom;
	updateableMatrix m_viewProj3DZoom;
	void m_checkAndUpdateZoom();
	size_t m_zoomUpdate = 0;
	//~unused


public:


	DrawData(const glm::f64mat4& view, const glm::f64mat4& projection2D, const PerspectiveProjection& projection3D);

	void setView(const glm::f64mat4 &view)noexcept;
	void setProjection2D(const glm::f64mat4 &projection2D)noexcept;
	void setProjection3D(const PerspectiveProjection &projection3D)noexcept;

	
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


	const PerspectiveProjection& getPerspectiveProjection()const noexcept;

	/// <summary>
	/// calculates the view matrix given the flags on-the-fly with no caching
	/// </summary>.
	/// <param name="what2Dor3D"></param>
	/// <param name="flags"></param>
	/// <returns></returns>
	glm::f64mat4 getView(DrawDataGetDimensions what2DOr3D, const std::vector<DrawDataGetFlags> &flags)const noexcept;

	/// <summary>
	/// a more sane version of the templated getViewProj function
	/// calculates the viewProj with the given flags on the fly applied in the order specified.
	/// </summary>
	/// <param name="flags">the flags to apply, make sure not to repeat them</param>
	/// <returns></returns>
	glm::f64mat4 getViewProj(DrawDataGetDimensions what2DOr3D, const std::vector<DrawDataGetFlags> &flags)const noexcept;

};
