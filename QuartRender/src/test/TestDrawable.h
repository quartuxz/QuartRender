#pragma once
#include "../core/drawables/IDrawable.h"
#include "../core/VertexArray.h"
#include "../core/IndexBuffer.h"
#include "../core/ShaderProgram.h"

#include "../core/drawables/IDrawable.h"

class TestDrawable final :
    public IDrawable
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
    bool setAndPop()override;

    DrawableTypes getDrawableType()const noexcept override;

    void draw(const DrawData& drawData)override;
};

