#include "PlanetDrawable.h"
#define _USE_MATH_DEFINES
#include <exception>
#include <math.h>
#include <type_traits>


//some test vertices and indices
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



static IndicesAndVertices_t* generateTestOutline(int, double) {

	IndicesAndVertices_t* retval = new IndicesAndVertices_t();
	retval->first = std::vector<GLfloat>(positions, positions + sizeof(positions)/sizeof(positions[0]));
	retval->second = std::vector<GLuint>(indices, indices + sizeof(indices)/sizeof(indices[0]));
	
	return retval;
}

//!!
//TODO: move ALL this to a more general file where it can be reused, candidate: DrawImplUtils.h
//!!

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

	//the position of the first point(point radius units above[+y dim] the "circle" centre)
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


//TODO: add an orbit eccentricity setting
/// <summary>
/// Creates a simple circular orbit without eccentricity(for now!)
/// </summary>
/// <param name="points">resolution of the orbit in terms of line segments</param>
/// <param name="radius">how large the orbit is</param>
/// <returns>an orbit line</returns>
static IndicesAndVertices_t* generateOrbit(unsigned int points, float radius) {
	IndicesAndVertices_t* retval;
	retval = new IndicesAndVertices_t();
	retval->first = generateCircleVertices(points, radius);
	//we create a simple 2d line to denote an orbit(with circle vertices and line indices)
	for (size_t i = 0; i < points-1; i++)
	{
		retval->second.push_back(i);
		retval->second.push_back(i+1);
	}

	retval->second.push_back(points-1);
	retval->second.push_back(0);
	return retval;
}


//TODO: factor out some of the sphere generation code to use to make line loops for orbits and
//outlines!

/// <summary>
/// NOTE: return values must be deleted by caller when done using.
/// </summary>
/// <param name="ringPoints"></param>
/// <param name="rings"></param>
/// <param name="radius"></param>
/// <param name="generateBothHalves"></param>
/// <returns></returns>
static IndicesAndVertices_t* generateSphere(unsigned int ringPoints, unsigned int rings, float radius, bool generateBothHalves = true) {
	
	if (rings%2==0) {
		throw std::invalid_argument("Must provide an odd number of rings!");
	}
	if (rings < 1) {
		throw std::invalid_argument("Rings must be greater than or equal to 1!");
	}

	//create the return value
	IndicesAndVertices_t* retval = new IndicesAndVertices_t();

	unsigned int halfRings = ceil(rings / (float)2);

	//reserve indices and vertices for both halves
	if (generateBothHalves) {
		//the total floats are three per vertex and there are as many vertices as
		//generated per ring+ the top and bottom poles as a single vertex each.
		//the middle ring is repeated once
		retval->first.reserve((rings+1) * ringPoints * 3 + 2 * 3);

		// the amount of indices needed to construct every square in the sphere
		//with two triangles per square.
		retval->second.reserve((rings-1) * ringPoints * 2 * 3+ringPoints*3*2);
	}
	//reserve indices and vertices for one half
	else {
		//the vertices
		//each ring contains ringPoints vertices and each vertex is 3 data float for position
		retval->first.reserve(halfRings*ringPoints*3+3);
		//the indices
		retval->second.reserve((halfRings-1)*ringPoints*2*3+ringPoints*3);
	}


	//we generate the middle ring
	std::vector<GLfloat> middleRing = generateCircleVertices(ringPoints,radius);
	retval->first.insert(retval->first.end(),middleRing.begin(), middleRing.end());

	
	//how many rings in a sphere half+1 to divide it accordingly.
	float zdisplace = radius / halfRings;

	for (unsigned int i = 1; i < halfRings; i++) {
		//the current z offset from the middle ring to the current.
		float currentZDisplace = zdisplace * i;
		//we apply pythagorean theorem to get the radius of the following ring
		float circleRadius = sqrt(radius*radius - currentZDisplace*currentZDisplace);
		// we create the circle vertices for the ring
		std::vector<GLfloat> circleVertices = generateCircleVertices(ringPoints,circleRadius,glm::vec3(0.0f,0.0f,currentZDisplace));
		//we add them to the return value
		retval->first.insert(retval->first.end(), circleVertices.begin(), circleVertices.end());
	}
	//we add the top vertex, and reserve no extra space as it is reserved beforehand.
	addVecToVector(retval->first, glm::vec3(0.0f,0.0f,radius),0);

	//this is also the last index of this sphere's half
	unsigned int topVertexIndex = ringPoints * halfRings;

	//NOTE: these indices also work for a 2D circlular line, useful for orbits and line outlines.
	//then we calculate the indices for the non-pole triangles
	for (unsigned int o = 0; o < (halfRings - 1); o++) {
		for (unsigned int i = o*ringPoints; i < (o+1)*ringPoints; i++) {
			if (i != (o+1)*(ringPoints)-1) {
				//first triangle of the square face
				retval->second.push_back(i);
				retval->second.push_back(i + ringPoints);
				retval->second.push_back(i + ringPoints + 1);
				//second triangle of the square face
				retval->second.push_back(i);
				retval->second.push_back(i + ringPoints + 1);
				retval->second.push_back(i + 1);
			}
			else {
				//conenct the two ends with a last square face
				//first triangle of the last square face
				retval->second.push_back(i);
				retval->second.push_back(i+ringPoints);
				retval->second.push_back(i+1);
				//second triangle of the last square face
				retval->second.push_back(i);
				retval->second.push_back(i+1);
				retval->second.push_back(o*ringPoints);
			}
		}
	}
	//we calculate the indices for the pole triangles
	for (unsigned int i = (ringPoints * (halfRings - 1)); i < topVertexIndex-1;i++) {
		retval->second.push_back(i);
		retval->second.push_back(topVertexIndex);
		retval->second.push_back(i+1);
	}
	//finally we tie both ends together.
	retval->second.push_back(topVertexIndex - 1);
	retval->second.push_back(topVertexIndex);
	retval->second.push_back(topVertexIndex - ringPoints);

	//creates the second half recursively.
	if (generateBothHalves) {
		// This could also be called secondHemisphere but I dont care, you name you sphere halves
		// as you please and ill do the same.(for the record, yes, they are both equivalent here,
		// just as you would divide the globe into south and north hemispheres they are divided here also
		// except the starting orientation of this sphere puts the polar axis parallel and coincident to the z axis)
		IndicesAndVertices_t* secondSphereHalf = generateSphere(ringPoints, rings, radius, false);
		
		//we simply reflect the vertices of the other half about the xy plane(mirroring on the other z side)
		//and also about the zy plane(flipping about the x axis to ensure normals still point outwards)
		//worth noting that the left-most point of the seconSphereHalf before reflection 
		//ends up meeting the right-most point of the first half after reflection.
		//and viceversa for the right-most point
		//care must be put on where the final vertices lie(on the second half) when assigning per vertex colour for example.
		for (size_t i = 0; i < secondSphereHalf->first.size(); i += 3) {
			retval->first.push_back(-secondSphereHalf->first[i]);
			retval->first.push_back(secondSphereHalf->first[i+1]);
			retval->first.push_back(-secondSphereHalf->first[i+2]);
		}

		/*
		if(false){
		//this is a very intensive-on-the-cpu routine for creating the second half of the sphere,
		//ideally I would add more code above this to do that(maybe putting the existing code in a 2 loop
		//and tweaking it to create the second halfSphere/hemisphere). This doesnt matter too much FOR NOW!
		//as these types of geometry are created usually once per drawable.
		//currently deprecated
		//NOTE: we can always cache them if many drawables create spheres with the same parameters!
		for (size_t i = 0; i < secondSphereHalf->first.size(); i+= 3) {
			//we take the current vertex
			glm::vec4 currentVertex(secondSphereHalf->first[i],secondSphereHalf->first[i+1],secondSphereHalf->first[i+2],1.0f);
			//apply a transformation of rotation in the y axis by 180 degrees, putting it opposite to the current halfSphere.
			currentVertex = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f,1.0f,0.0f))*currentVertex;
			//add the rotated vertices to the retval
			addVecToVector(retval->first, glm::vec3(currentVertex.x,currentVertex.y,currentVertex.z),0);
		}
		}
		*/
		
		for (auto x: secondSphereHalf->second) {
			//we just take the existing indices and add an offset so they can be applied to the existing vertices and new ones.
			retval->second.push_back(x+topVertexIndex+1);
		}
		//we no longer use the halfSphere, all of its values are transformed and copied to the first.
		delete secondSphereHalf;
	}


	return retval;
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
	//each new point forms a triangle that is composed of 3 vertices, thus there are 3 indices per point
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

//TODO:
//Determine minimum size for an outline on-screen(from a practical point of view, e.g make it too small and its difficult to see)
//Determine maximum size in the same way
//Let minimum be called s and maximum S
//implement a function that takes a physical size(realSize) and satisfies lim x->inf f(x)<=S
//and also that lim x->0+ f(x)>=s,
//let it be continuous in (1/inf,inf), let f'(x) > 0 and f(x) >= s for all X in (1/inf, inf)
//let it be undefined when x<=0 and let the c++ function throw an exception.
double createScreenSizeForPlanet(double realSize) {
	if (realSize<=0) {
		throw std::invalid_argument("realSize must be greater than 0.");
	}
	double retval = 0.5;
	return retval;
}


PlanetDrawable::PlanetDrawable(const PlanetCharacteristics& characteristics) :
	m_characteristics(characteristics),
	m_screenSizeForPlanetOutline(createScreenSizeForPlanet(characteristics.radius)),
	//initialize the outline vertices, indices and shaders
	//TODO: tweak and set the parameters for point resolution of the circles and radius(for all circular drawables)
	m_outlineVerticesAndIndices(generatePlanetOutLine(30, 1.0)),
	m_outlineIndexBuffer(m_outlineVerticesAndIndices->second.data(), m_outlineVerticesAndIndices->second.size()),
	m_outlineProgram("./render_res/shaders/flatTestVert.glsl", "./render_res/shaders/flatTestFrag.glsl"),
	m_outlineVertexBuffer(m_outlineVerticesAndIndices->first.data(), m_outlineVerticesAndIndices->first.size() * sizeof(GLfloat)),
	//m_outlineVertexBuffer(positions, 2 * 4 * sizeof(GLfloat)),
	//m_outlineIndexBuffer(indices, 6)
	//initialize all the sphere  vertices, indices and shaders
	//TODO: maybe move these values for sphere resoultion to the settings class.
	m_planetSphereVerticesAndIndices(generateSphere(40, 41, 1.0)),
	m_planetSphereIndexBuffer(m_planetSphereVerticesAndIndices->second.data(), m_planetSphereVerticesAndIndices->second.size()),
	m_planetSphereProgram("./render_res/shaders/planetSphereVert.glsl", "./render_res/shaders/planetSphereFrag.glsl"),
	m_planetSphereVertexBuffer(m_planetSphereVerticesAndIndices->first.data(), m_planetSphereVerticesAndIndices->first.size() * sizeof(GLfloat)),

	//initialize all the orbit vertices, indices and shaders
	m_orbitLineVerticesAndIndicies(generateOrbit(10, 1)),
	m_orbitLineIndexBuffer(m_orbitLineVerticesAndIndicies->second.data(), m_orbitLineVerticesAndIndicies->second.size()),
	m_orbitLineProgram("./render_res/shaders/flatTestVert.glsl", "./render_res/shaders/flatTestFrag.glsl"),
	m_orbitLineVertexBuffer(m_orbitLineVerticesAndIndicies->first.data(), m_orbitLineVerticesAndIndicies->first.size()*sizeof(GLfloat))

{
	BufferLayout outlineLayout;
	outlineLayout.addBufferLayoutElement(BufferLayoutElement(3,GL_FLOAT,GL_FALSE));
	m_outlineVertexArray.addBuffer(&m_outlineVertexBuffer, outlineLayout);

	u_color = m_outlineProgram.getUniformHandle<UniformTypes::FLOAT_4>("u_color");
	u_MVP = m_outlineProgram.getUniformHandle<UniformTypes::FLOAT_MAT_4x4>("u_MVP");

	//the same as above, they are separated for future changes if any(may add vertex colour to the sphere)
	BufferLayout sphereLayout;
	sphereLayout.addBufferLayoutElement(BufferLayoutElement(3,GL_FLOAT,GL_FALSE));
	m_planetSphereVertexArray.addBuffer(&m_planetSphereVertexBuffer, sphereLayout);

	u_3DMVP = m_planetSphereProgram.getUniformHandle<UniformTypes::FLOAT_MAT_4x4>("u_3DMVP");
	u_model = m_planetSphereProgram.getUniformHandle<UniformTypes::FLOAT_MAT_4x4>("u_model");
	
	//create the bufferLayout for the orbit
	BufferLayout orbitLineLayout;
	orbitLineLayout.addBufferLayoutElement(BufferLayoutElement(3, GL_FLOAT, GL_FALSE));
	m_orbitLineVertexArray.addBuffer(&m_orbitLineVertexBuffer, orbitLineLayout);
	
	u_orbitLineColor = m_orbitLineProgram.getUniformHandle<UniformTypes::FLOAT_4>("u_color");
	u_orbitLineMVP = m_orbitLineProgram.getUniformHandle<UniformTypes::FLOAT_MAT_4x4>("u_MVP");
	
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

	//selectively draw the planet outline or the 3d planet sphere depending on zoom level
	if (m_screenSizeForPlanetOutline > ((m_characteristics.radius * drawData.getZoom()) / tan(drawData.getPerspectiveProjection().getFOVX() / 2))) {
		

		//unscales the shape so that is does not change in size when zooming as it behaves more like a UI
		//element than an actual graphical entity(a 2d planet is just there for visualization)
		glm::f64mat4 new2DModelTransform = m_drawVariation.modelTransform2D3DHybrid.modelTransform2D*glm::scale(glm::f64mat4(1.0f), glm::f64vec3(1 / drawData.getZoom(), 1 / drawData.getZoom(), 1))* glm::scale(glm::f64mat4(1.0f), glm::f64vec3(m_screenSizeForPlanetOutline));
		glm::f64mat4 mvp = drawData.getViewProj(DrawDataGetDimensions::get2D, { DEFAULT_DRAW_GET_FLAGS }) * new2DModelTransform;
		
		//setting full opacity
		u_color->setUniform(0.8f, 0.8f, 0.6f, 0.5f);
		//drawing the planet outline
		u_MVP->setUniform(mvp);

		m_outlineProgram.bind();
		m_outlineVertexArray.bind();
		m_outlineIndexBuffer.bind();
		THROW_ERRORS_GL_FAST(glDrawElements(GL_TRIANGLES, m_outlineIndexBuffer.getCount(), GL_UNSIGNED_INT, nullptr));
	}
	else {
		//drawing the sphere
		glm::f64mat4 mvp3D = drawData.getViewProj(DrawDataGetDimensions::get3D, { DrawDataGetFlags::getDisplacement, DrawDataGetFlags::getZoomed }) * m_drawVariation.modelTransform2D3DHybrid.modelTransform3D * glm::scale(glm::f64mat4(1.0f), glm::f64vec3(m_characteristics.radius));

		u_3DMVP->setUniform(mvp3D);
		u_model->setUniform(m_drawVariation.modelTransform2D3DHybrid.modelTransform3D);

		m_planetSphereProgram.bind();
		m_planetSphereVertexArray.bind();
		m_planetSphereIndexBuffer.bind();
		THROW_ERRORS_GL_FAST(glDrawElements(GL_TRIANGLES, m_planetSphereIndexBuffer.getCount(), GL_UNSIGNED_INT, nullptr));
	}


	//TODO: move orbit drawing to separate Drawable
	//AND figure out how to draw an orbit line of constant width for any given zoom value
	//drawing orbit
	glm::f64mat4 orbitLineMVP = drawData.getViewProj(DrawDataGetDimensions::get2D, {DEFAULT_DRAW_GET_FLAGS});

	THROW_ERRORS_GL_FAST(glLineWidth(5));
	u_orbitLineColor->setUniform(0.4f, 0.4f, 0.8f, 1.0f);
	u_orbitLineMVP->setUniform(orbitLineMVP);

	m_orbitLineProgram.bind();
	m_orbitLineVertexArray.bind();
	m_orbitLineIndexBuffer.bind();
	//temporarlily disabled obrit drawing
	//THROW_ERRORS_GL_FAST(glDrawElements(GL_LINES, m_orbitLineIndexBuffer.getCount(), GL_UNSIGNED_INT, nullptr));



}

PlanetDrawable::~PlanetDrawable()
{
	delete m_outlineVerticesAndIndices;
	delete m_planetSphereVerticesAndIndices;
}
