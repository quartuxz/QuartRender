#pragma once
#include "../core/drawables/IDrawable.h"
#include "../core/VertexArray.h"
#include "../core/IndexBuffer.h"
#include "../core/ShaderProgram.h"

#include "../core/drawables/IVariableDrawable.h"

class TestDrawable final :
    public IDrawable,
    public IVariableDrawable<glm::mat4>
{
private:
    VertexArray vertexArray;
    IndexBuffer indexBuffer;
    ShaderProgram program;
    VertexBuffer vertexBuffer;

    UniformGL<UniformTypes::FLOAT_4>* u_color;
    UniformGL<UniformTypes::FLOAT_MAT_4x4>* u_MVP;
public:
    TestDrawable();

    void addVariation(const glm::mat4& modelTrans)override;
    bool setAndPop()override;

    DrawableTypes getDrawableType()override;

    void draw(const DrawData& drawData)override;
};

