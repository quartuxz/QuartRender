#include "TestDrawable.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


static const GLfloat positions[] = {
-0.5f ,-0.5f, 0.0f, 0.0f,
0.5f ,-0.5f, 1.0f, 0.0f,
0.5f ,0.5f, 1.0f,1.0f,
-0.5f ,0.5f, 0.0f,1.0f
};

static const GLuint indices[] = {
    0,1,2,
    2,3,0
};


TestDrawable::TestDrawable() :
    indexBuffer(indices, 6),
    program("./render_res/shaders/flatTestVert.glsl", "./render_res/shaders/flatTestFrag.glsl"),
    vertexBuffer(positions, 4 * 4 * sizeof(GLfloat))
{
    LOG_TO_CONSOLE_COND("test drawable object constructor called!");
    BufferLayout layout;
    layout.addBufferLayoutElement(BufferLayoutElement(2, GL_FLOAT, GL_FALSE));
    layout.addBufferLayoutElement(BufferLayoutElement(2, GL_FLOAT, GL_FALSE));
    vertexArray.addBuffer(&vertexBuffer, layout);

    u_color = program.getUniformHandle<UniformTypes::FLOAT_4>("u_color");
    u_MVP = program.getUniformHandle<UniformTypes::FLOAT_MAT_4x4>("u_MVP");
}

bool TestDrawable::setAndPop()
{
    return IDrawable::setAndPop();
}

DrawableTypes TestDrawable::getDrawableType()const noexcept
{
    return DrawableTypes::drawable2D;
}

void TestDrawable::draw(const DrawData& drawData)
{
    LOG_TO_CONSOLE_COND("draw started.");

    glm::f64mat4 mvp = drawData.getViewProj(DrawDataGetDimensions::get2D,{DEFAULT_DRAW_GET_FLAGS}) * m_drawVariation.modelTransform;

    LOG_TO_CONSOLE_COND("mvp calculated.");
    u_color->setUniform(0.5f, 0.5f, 0.7f, 1.0f);
    u_MVP->setUniform(mvp);
    LOG_TO_CONSOLE_COND("uniforms set.");

    program.bind();
    vertexArray.bind();
    indexBuffer.bind();
    LOG_TO_CONSOLE_COND("program, indexBuffer and vertexArray bound.");
    THROW_ERRORS_GL_FAST(glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr));
    LOG_TO_CONSOLE_COND("drawn.");
}
