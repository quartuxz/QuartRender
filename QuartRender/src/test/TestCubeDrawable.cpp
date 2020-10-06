#include "TestCubeDrawable.h"
#define _USE_MATH_DEFINES

#include <random>
#include <chrono>

#include <cmath>
#include <vector>



//both cubeVertices and cubeIndices are here to create a perfect square to test my algorithm against
static const GLfloat cubeVertices[] =
{
	-1, -1, -1,            1.0f, 1.0f, 1.0f, 
	1, -1, -1,            1.0f, 1.0f, 1.0f,
	1, 1, -1,            1.0f, 1.0f, 1.0f,
	-1, 1, -1,            1.0f, 1.0f, 1.0f,
	-1, -1, 1,            1.0f, 1.0f, 1.0f,
	1, -1, 1,            1.0f, 1.0f, 1.0f,
	1, 1, 1,            1.0f, 1.0f, 1.0f,
	-1, 1, 1,            1.0f, 1.0f, 1.0f
};
static const GLfloat cubeVertices_noColour[] =
{
	-1, -1, -1,           
	1, -1, -1,            
	1, 1, -1,            
	-1, 1, -1,           
	-1, -1, 1,           
	1, -1, 1,           
	1, 1, 1,          
	-1, 1, 1,            
};

static const GLuint cubeIndices[] =
{
	0, 1, 3,
	3, 1, 2,
	1, 5, 2,
	2, 5, 6,
	5, 4, 6,
	6, 4, 7,
	4, 0, 7,
	7, 0, 3,
	3, 2, 7,
	7, 2, 6,
	4, 5, 0,
	0, 5, 1
};


IndicesAndVertices_t* generateCube(float distanceToFace) {
	IndicesAndVertices_t* retval = new IndicesAndVertices_t();

	//create a random number generator
	std::default_random_engine rand_generator;
	rand_generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<double> rand_distribution(0, 1);

	//manually set the indices(there may be a better way in a loop but im too lazy to think of one)
	static const GLuint indices[] = {
		//side faces
		0,2,3,
		1,0,3,
		1,3,5,
		5,3,7,
		5,7,4,
		4,7,6,
		4,2,6,
		0,2,4,
		//top face
		2,7,3,
		2,6,7,
		//bottom face
		0,4,5,
		0,5,1
	};


	//each face contains 6 indices for two triangles(3 per triangle... duh!)
	//so... 36 indices
	retval->second = std::vector<GLuint>(indices, indices + 36);


	//there are 8 vertices in a cube, each vertex has 3 floats for xy and z and 3 floats for colour
	retval->first.reserve(8*3*2);



	//these nested for loops iterate a total of 8 times over the contained code
	//the code alternates between negative and positive values of xy and z to generate 
	//positions for the vertices relative to the "centre" of the cube.
	for (int x = -1; x < 2; x += 2) {
		for (int y = -1; y < 2; y += 2) {
			for (int z = -1; z < 2; z += 2) {
				//a total of 3 floats per vertex correesponding to each dimension.
				retval->first.push_back(x * distanceToFace);
				retval->first.push_back(y * distanceToFace);
				//TODO: REMOVE THE +1.0f ITS THERE FOR TESTING
				//^^done, but may add it later
				retval->first.push_back(z * distanceToFace);

				//then we do random color generation, 3 floats per vertex representing rgb
				for (size_t i = 0; i < 3; i++)
				{
					retval->first.push_back(rand_distribution(rand_generator));
				}
			}
		}
	}


	return retval;
}


TestCubeDrawable::TestCubeDrawable():
	m_cubeVerticesAndIndices(generateCube(1.0f)),
	//m_cubeVerticesAndIndices(new IndicesAndVertices_t(std::vector<GLfloat>(cubeVertices, cubeVertices+48), std::vector<GLuint>(cubeIndices,cubeIndices+36))),
	m_IndexBuffer(m_cubeVerticesAndIndices->second.data(), m_cubeVerticesAndIndices->second.size()),
	m_Program("./render_res/shaders/cubeVert.glsl", "./render_res/shaders/cubeFrag.glsl"),
	m_VertexBuffer(m_cubeVerticesAndIndices->first.data(), m_cubeVerticesAndIndices->first.size()*sizeof(GLfloat))
{
	LOG_TO_CONSOLE("test cube drawable constructor initialization ended, body started!");
	BufferLayout layout;
	//we bind first the 3 position determining floats for xy and z.
	layout.addBufferLayoutElement(BufferLayoutElement(3,GL_FLOAT,GL_FALSE));
	//then the 3 elements of a color per vertex.
	layout.addBufferLayoutElement(BufferLayoutElement(3,GL_FLOAT,GL_FALSE));
	m_VertexArray.addBuffer(&m_VertexBuffer, layout);

	u_MVP = m_Program.getUniformHandle<UniformTypes::FLOAT_MAT_4x4>("u_MVP");

	LOG_TO_CONSOLE("test cube drawable constructor body ended!");
}

DrawableTypes TestCubeDrawable::getDrawableType() const noexcept
{
	return DrawableTypes::drawable3D;
}

void TestCubeDrawable::draw(const DrawData& drawData)
{
	LOG_TO_CONSOLE("draw started!");
	m_drawVariation.modelTransform;
	glm::f64mat4 mvp = drawData.getViewProj(DrawDataGetDimensions::get3D, {DEFAULT_DRAW_GET_FLAGS})*m_drawVariation.modelTransform;

	LOG_TO_CONSOLE("mvp calculated!");

	u_MVP->setUniform(mvp);

	LOG_TO_CONSOLE("uniform set!");

	m_Program.bind();
	m_VertexArray.bind();
	m_IndexBuffer.bind();
	THROW_ERRORS_GL_FAST(glDrawElements(GL_TRIANGLES, m_IndexBuffer.getCount(), GL_UNSIGNED_INT, nullptr));
}

TestCubeDrawable::~TestCubeDrawable()
{
	delete m_cubeVerticesAndIndices;
}
