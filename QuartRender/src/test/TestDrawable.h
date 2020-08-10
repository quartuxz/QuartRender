#pragma once
#include "../core/drawables/IDrawable.h"
#include "../core/VertexArray.h"
#include "../core/IndexBuffer.h"
#include "../core/ShaderProgram.h"

#include "../core/drawables/IVariableDrawable.h"

class TestDrawable :
    public IDrawable,
    public IVariableDrawable<glm::mat4>
{
private:
    VertexArray vertexArray;
    IndexBuffer indexBuffer;
    ShaderProgram program;
    VertexBuffer vertexBuffer;

public:
    TestDrawable();

    void addVariation(const glm::mat4& modelTrans)override;
    bool setAndPop()override;

    DrawableTypes getDrawableType()override;

    void draw(glm::mat4 viewProj)override;
};

