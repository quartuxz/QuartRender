#include "TestCubeDrawable.h"
#define _USE_MATH_DEFINES

#include <random>
#include <chrono>
#include <stdexcept>
#include <cmath>
#include <vector>



//both cubeVertices and cubeIndices are here to create a perfect square to test my algorithm against
static const GLfloat cubeVertices[] =
{
	-1, -1, -1,            1.0f, 0.5f, 0.2f, 
	1, -1, -1,            0.0f, 1.0f, 0.4f,
	1, 1, -1,            0.5f, 1.0f, 1.0f,
	-1, 1, -1,            0.5f, 1.0f, 1.0f,
	-1, -1, 1,            1.0f, 1.0f, 1.0f,
	1, -1, 1,            1.0f, 0.5f, 1.0f,
	1, 1, 1,            1.0f, 0.5f, 1.0f,
	-1, 1, 1,            1.0f, 1.0f, 0.5f
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



static IndicesAndVertices_t* generateCube(float distanceToFace) {
	if (distanceToFace < 0.0f) {
		throw std::invalid_argument("Distance to face must be greater than 0!");
	}
	
	IndicesAndVertices_t* retval = new IndicesAndVertices_t();

	//create a random number generator
	std::default_random_engine rand_generator;
	rand_generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<double> rand_distribution(0, 1);

	//manually set the indices(there may be a better way in a loop but im too lazy to think of one)
	static const GLuint indices[] = {
		//left face(if I where the cube and I where facing in the same direction of the front face,
		//it would be my right side actually, but since im instead facing against it in the way its drawn, its the left)
		3,2,0,
		1,3,0,
		//right(same applies as above)
		7,5,4,
		7,4,6,
		//front face 
		22,23,21,
		20,22,21,
		//back face 
		17,18,16,
		18,17,19,
		//top face
		12,13,15,
		15,14,12,
		//bottom face
		8,10,9,
		11,9,10
	};


	//each face contains 6 indices for two triangles(3 per triangle... duh!)
	//so... 36 indices
	retval->second = std::vector<GLuint>(indices, indices + 36);


	//there are 6 faces in a cube, each face has 4 independent vertices, each vertex has 3 floats
	//por position, 3 for normals, and 3 for colour in that order
	retval->first.reserve(24*3*3);


	//we do a loop for every dimension
	for (size_t i = 0; i < 3; i++)
	{
		//in both positive and negative values
		for (int o = -1; o < 2; o+=2)
		{
			//this is the normal for the entire face(all the vertices)
			glm::vec3 normal = {0.0f,0.0f,0.0f};
			normal[i] = o;
			

			for (int j = -1; j < 2; j += 2) {
				for (int k = -1; k < 2; k+=2) {
					//we generate the vertices
					//the x value
					retval->first.push_back((i==0?o:j)*distanceToFace);
					//the y value
					retval->first.push_back((i==1?o:(i==2?k:j))*distanceToFace);
					//the z value
					retval->first.push_back((i==2?o:k)*distanceToFace);

					//we load the normals
					retval->first.push_back(normal.x);
					retval->first.push_back(normal.y);
					retval->first.push_back(normal.z);

					//then we do random color generation, 3 floats per vertex representing rgb
					for (size_t l = 0; l < 3; l++)
					{
						retval->first.push_back(rand_distribution(rand_generator));
					}

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
	BufferLayout layout;
	//we bind first the 3 position determining floats for xy and z.
	layout.addBufferLayoutElement(BufferLayoutElement(3,GL_FLOAT,GL_FALSE));
	//then the normals
	layout.addBufferLayoutElement(BufferLayoutElement(3,GL_FLOAT,GL_FALSE));
	//finally the 3 elements of a color per vertex.
	layout.addBufferLayoutElement(BufferLayoutElement(3, GL_FLOAT, GL_FALSE));
	m_VertexArray.addBuffer(&m_VertexBuffer, layout);

	u_MVP = m_Program.getUniformHandle<UniformTypes::FLOAT_MAT_4x4>("u_MVP");
	u_model = m_Program.getUniformHandle<UniformTypes::FLOAT_MAT_4x4>("u_model");

}

DrawableTypes TestCubeDrawable::getDrawableType() const noexcept
{
	return DrawableTypes::drawable3D;
}

void TestCubeDrawable::draw(const DrawData& drawData)
{
	glm::f64mat4 mvp = drawData.getViewProj(DrawDataGetDimensions::get3D, {DEFAULT_DRAW_GET_FLAGS})*m_drawVariation.modelTransform;


	u_MVP->setUniform(mvp);
	u_model->setUniform(m_drawVariation.modelTransform);

	m_Program.bind();
	m_VertexArray.bind();
	m_IndexBuffer.bind();
	THROW_ERRORS_GL_FAST(glDrawElements(GL_TRIANGLES, m_IndexBuffer.getCount(), GL_UNSIGNED_INT, nullptr));
}

TestCubeDrawable::~TestCubeDrawable()
{
	delete m_cubeVerticesAndIndices;
}
