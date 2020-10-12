#include "PlanetDrawable.h"
#define _USE_MATH_DEFINES
#include <exception>
#include <math.h>
#include <type_traits>

static const GLfloat positions[] = {
-0.5f ,-0.5f,
0.5f ,-0.5f,
0.5f ,0.5f,
-0.5f ,0.5f
};

static const GLuint indices[] = {
	0,1,2,
	2,3,0
};



static std::pair<std::vector<GLfloat>, std::vector<GLuint>>* generateTestOutline(int, double) {
	typedef std::remove_reference<decltype(*generateTestOutline(0, 0))>::type rettype;

	rettype* retval = new rettype();
	retval->first = std::vector<GLfloat>(positions, positions + sizeof(positions)/sizeof(positions[0]));
	retval->second = std::vector<GLuint>(indices, indices + sizeof(indices)/sizeof(indices[0]));
	
	return retval;
}


//TODO: move this to a more general file where it can be reused, candidate: DrawImplUtils.h
//(remember to mark as inline and remove static).
/// <summary>
/// generates a circle of vertices, with points*3total GLfloats in the return value.
/// </summary>
/// <param name="points">>3, how much resolution the circle has.</param>
/// <param name="radius">>0, how large is the distance from the centre to any of the vertices.</param>
/// <param name="displacement">an added value to the position of every vertex.</param>
/// <returns></returns>
static std::vector<GLfloat> generateCircleVertices(unsigned int points, float radius, glm::vec3 displacement = glm::vec3(0.0f,0.0f,0.0f)) {
	if (points < 3) {
		throw std::invalid_argument("You must provide a number greater than 2 for points!");
	}

	if (radius <= 0) {
		throw std::invalid_argument("Radius must be greater than 0!");
	}

	std::vector<GLfloat> retval;

	//reserve the total amount of vertex data(3 floats per vertex for x, y and z)
	retval.reserve(points * 3);

	//calculate the angle between consecutive lines running from the center
	double perPointAngle = (2 * M_PI) / points;

	//the position of the first point(point radius units above the "circle" centre)
	float xfirst = displacement.x, yfirst = radius+displacement.y, zfirst = displacement.z;
	//we add the first point as a vertex
	retval.push_back(xfirst);
	retval.push_back(yfirst);
	retval.push_back(zfirst);

	//we create an isoceles triangle to determine the position of each point relative to the first point,
	//then we calculate all of its angles and sides to determine the lengths of the sides of a right
	//triangle sharing the base of the isoceles triangle and going from the first point to the current,
	//with a 90 degree angle.
	for (size_t i = 1; i < points; i++)
	{
		//determine the angle between the first point and the current point i
		double currentAngleOffset = perPointAngle * i;

		bool currentAngleOffsetIsGreaterThanPI = currentAngleOffset > M_PI;

		//determine the shortest of both angles between first point and the current point i
		double shortestAngleOffset = currentAngleOffset;
		if (currentAngleOffsetIsGreaterThanPI) {
			shortestAngleOffset = (2 * M_PI) - shortestAngleOffset;
		}

		//determine the length of the isoceles triangle formed between the center of the circle, the first point,
		//and the current point i
		double isoscelesBase = radius * sqrt(2 - 2 * (cos(shortestAngleOffset)));
		//determine top angle of right angle triangle sharing isocelesBase, axis aligned
		double topAngle = (M_PI / 2) - ((M_PI - shortestAngleOffset) / 2);
		//determine final offsets for position of new point
		float xoffset = isoscelesBase * cos(topAngle);
		float yoffset = isoscelesBase * sin(topAngle);
		//generate the vertices
		if (currentAngleOffsetIsGreaterThanPI) {
			retval.push_back(xfirst - xoffset);
		}
		else {
			retval.push_back(xfirst + xoffset);
		}

		retval.push_back(yfirst - yoffset);
		//everything is on the same z-plane
		retval.push_back(zfirst);
	}

	return retval;
}


static IndicesAndVertices_t* generateSphere(unsigned int ringPoints, unsigned int rings, float radius) {
	
	glm::vec3 centerPoint = glm::vec3(0.0f, 0.0f, 0.0f);

}


//creates a 2d outline representation of vertices and indices for drawing, with the given amount of
//points(>2) and radius(>0)
static 	IndicesAndVertices_t* generatePlanetOutLine(unsigned int points, float radius) {
	
	IndicesAndVertices_t* retval = new 	IndicesAndVertices_t();

	//add the circle centre vertex
	addVecToVector(retval->first, glm::vec3(0.0f,0.0f,0.0f));
	const auto& circleVertices = generateCircleVertices(points, radius);
	retval->first.insert(retval->first.end(),circleVertices.begin(),circleVertices.end());

	//reserve total amount of index data,
	//each new pòint forms a triangle that is composed of 3 vertices, thus there are 3 indices per point
	//(might be more or less this is just a rough reserve, didnt do the math properly)
	retval->second.reserve(points*3);


	
	for (size_t i = 1; i < points; i++)
	{
	
		//generate the indices in counter-clockwise order
		//first the centre
		retval->second.push_back(0);
		//then the current
		retval->second.push_back(i+1);
		//then the last
		//(keep in mind the current is offset by one from i since the two first vertices
		//are the centre first and the first point second-> i starts from 1[first point] so the current point
		//is i+1)
		retval->second.push_back(i);
		
	}

	//tie both ends together in clockwise order
	//first the centre
	retval->second.push_back(0);
	//then the last point generated
	retval->second.push_back(1);
	//then the first point
	retval->second.push_back(points);


	return retval;
}



PlanetDrawable::PlanetDrawable(const PlanetCharacteristics& characteristics):
	m_characteristics(characteristics),
	m_outlineVerticesAndIndices(generatePlanetOutLine(30,0.2)),

	m_outlineIndexBuffer(m_outlineVerticesAndIndices->second.data(), m_outlineVerticesAndIndices->second.size()),
	m_outlineProgram("./render_res/shaders/flatTestVert.glsl", "./render_res/shaders/flatTestFrag.glsl"),
	m_outlineVertexBuffer(m_outlineVerticesAndIndices->first.data(), m_outlineVerticesAndIndices->first.size()*sizeof(GLfloat))
	//m_outlineVertexBuffer(positions, 2 * 4 * sizeof(GLfloat)),
	//m_outlineIndexBuffer(indices, 6)
{
	BufferLayout layout;
	layout.addBufferLayoutElement(BufferLayoutElement(3,GL_FLOAT,GL_FALSE));
	m_outlineVertexArray.addBuffer(&m_outlineVertexBuffer, layout);

	u_color = m_outlineProgram.getUniformHandle<UniformTypes::FLOAT_4>("u_color");
	u_MVP = m_outlineProgram.getUniformHandle<UniformTypes::FLOAT_MAT_4x4>("u_MVP");
}

bool PlanetDrawable::setAndPop()
{
	return 	IDrawable::setAndPop();
}

DrawableTypes PlanetDrawable::getDrawableType() const noexcept
{
	return DrawableTypes::drawable2D;
}

void PlanetDrawable::draw(const DrawData& drawData)
{
	//select default one in template(2d)
	
	//unscales the shape so that is does not change in size when zooming as it behaves more like a UI
	//element than an actual graphical entity(a 2d planet is just there for visualization)
	m_drawVariation.modelTransform = glm::scale(m_drawVariation.modelTransform, glm::f64vec3(1/drawData.getZoom(),1/drawData.getZoom(),1));
	
	glm::f64mat4 mvp = drawData.getViewProj(DrawDataGetDimensions::get2D,{DEFAULT_DRAW_GET_FLAGS}) * m_drawVariation.modelTransform;


	u_color->setUniform(0.8f, 0.8f, 0.6f, 1.0f);
	u_MVP->setUniform(mvp);

	m_outlineProgram.bind();
	m_outlineVertexArray.bind();
	m_outlineIndexBuffer.bind();
	THROW_ERRORS_GL_FAST(glDrawElements(GL_TRIANGLES, m_outlineIndexBuffer.getCount(), GL_UNSIGNED_INT, nullptr));
}

PlanetDrawable::~PlanetDrawable()
{
	delete m_outlineVerticesAndIndices;
}
