#include "DrawDataStructs.h"
#include <algorithm>

//unused
void DrawData::m_checkAndUpdateZoom()
{
}


//TODO:lots of things in this implementation are unused, delete them or replace them.




DrawData::DrawData(const glm::f64mat4& view, const glm::f64mat4& projection2D, const PerspectiveProjection& projection3D) :
	m_view(view),
	m_projection2D(projection2D),
	m_projection3D(projection3D)
{
}

void DrawData::setView(const glm::f64mat4& view) noexcept
{
	m_view = view;
}

void DrawData::setProjection2D(const glm::f64mat4& projection2D) noexcept {
	m_projection2D = projection2D;
}

void DrawData::setProjection3D(const PerspectiveProjection& projection3D) noexcept
{
	m_projection3D = projection3D;
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
		return m_projection3D.getMatrix();
	}
}

const PerspectiveProjection& DrawData::getPerspectiveProjection() const noexcept
{
	return m_projection3D;
}

glm::f64mat4 DrawData::getView(DrawDataGetDimensions what2DOr3D, const std::vector<DrawDataGetFlags>& flags) const noexcept
{
	glm::f64mat4 retval = m_view;

	bool wasZoomed3D = false;

	//TODO: make 3d view transformations work down here
	for (auto x : flags) {
		switch (x) {
		case DrawDataGetFlags::getZoomed:
			if (what2DOr3D == DrawDataGetDimensions::get2D) {
				glm::f64mat4 zoomMat = glm::scale(glm::f64mat4(1.0), glm::f64vec3(m_zoomLevel, m_zoomLevel, 1));
				retval = zoomMat * retval;
			}
			else {
				glm::f64mat4 zoomMat = glm::scale(glm::f64mat4(1.0), glm::f64vec3(m_zoomLevel, m_zoomLevel, m_zoomLevel));
				retval = zoomMat * retval;
			}
			break;
		case DrawDataGetFlags::getZPlaneZoomed:
			//only works for 3d
			if (what2DOr3D == DrawDataGetDimensions::get3D) {
				wasZoomed3D = true;
				//we simply translate the 3d objects in the z direction to make them appear "zoomed in" or larger to the camara
				//since xy are divided by z, we translate z by 1/zoomLevel, thus (x or y) / (1/z) = (x or y)*z, we achieve scaling
				//I guess directly scaling by zoomLevel would have the same effect
				//we used a scalar that indicates the nominal distance from everything(or a background back-most "plane" parallel to xy)
				// this nominal distance is the added z translate to all geometry when zoomLevel is 1
				//it has the name of m_initial3DZDisplacement
				glm::f64mat4 zoomMat = glm::translate(glm::f64mat4(1.0), glm::f64vec3(0.0, 0.0, -m_initial3DZDisplacement / m_zoomLevel));
				retval = zoomMat * retval;
			}
			break;
		case DrawDataGetFlags::getDisplacement:
			if (what2DOr3D == DrawDataGetDimensions::get2D) {
				//copy assingment probably slower than direct multiplication with rvalue, I dont care for now
				//same happens in other places nearby
				glm::f64mat4 displacementMat = glm::translate(glm::f64mat4(1.0f), glm::f64vec3(m_displacement.x,m_displacement.y,0.0f));
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

	if (!wasZoomed3D && what2DOr3D == DrawDataGetDimensions::get3D) {
		//we translate by the m_initial3DZDisplacement to be consistent with an "unzoomed" space
		//this is the same as if we would set the zoomLevel to 1 and request zoom to be applied to the
		//view matrix above
		//this translation is also necesary as a basis for all 3D drawings, they require a distance from the POV
		//to actually show all of their geometry, this number should go into some program-wide-settings class.
		glm::f64mat4 noZoomMat = glm::translate(glm::f64mat4(1.0), glm::f64vec3(0.0, 0.0, -m_initial3DZDisplacement));
		retval = noZoomMat * retval;
	}

	return retval;

}

glm::f64mat4 DrawData::getViewProj(DrawDataGetDimensions what2DOr3D, const std::vector<DrawDataGetFlags>& flags)const noexcept
{
	glm::f64mat4 retval = getView(what2DOr3D, flags);

	if (what2DOr3D == DrawDataGetDimensions::get2D) {
		retval = m_projection2D * retval;
	}
	else {
		if (std::find(flags.begin(), flags.end(),DrawDataGetFlags::getFOVZoomed)!=flags.end()) {
			retval = PerspectiveProjection(atan(tan(m_projection3D.getFOVY()) / m_zoomLevel),m_projection3D).getMatrix()*retval;
		}
		else {
			retval = m_projection3D.getMatrix() * retval;
		}


	}

	return retval;
}

PerspectiveProjection::PerspectiveProjection(float FOVY, float aspect, float near, float far) :
	m_FOVY(FOVY),
	m_aspect(aspect),
	m_near(near),
	m_far(far),
	m_mat(glm::perspective(FOVY, aspect, near, far))
{
}

PerspectiveProjection::PerspectiveProjection(float FOVY, const PerspectiveProjection& other):
	m_FOVY(FOVY),
	m_aspect(other.m_aspect),
	m_near(other.m_near),
	m_far(other.m_far),
	m_mat(glm::perspective(m_FOVY, m_aspect, m_near, m_far))
{
}

float PerspectiveProjection::getFOVY() const noexcept
{
	return m_FOVY;
}

float PerspectiveProjection::getFOVX() const noexcept
{
	return m_FOVY*m_aspect;
}

float PerspectiveProjection::getAspect() const noexcept
{
	return m_aspect;
}

float PerspectiveProjection::getNear() const noexcept
{
	return m_near;
}

float PerspectiveProjection::getFar() const noexcept
{
	return m_far;
}

const glm::f64mat4& PerspectiveProjection::getMatrix() const noexcept
{
	return m_mat;
}
