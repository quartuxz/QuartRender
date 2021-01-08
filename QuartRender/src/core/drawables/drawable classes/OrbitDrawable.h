#pragma once
#include <vector>
#include "../IDrawable.h"
#include "../../DrawImplUtils.h"

struct OrbitCharacteristics {
    std::vector<glm::f64vec3> points;
};

class OrbitDrawable :
    public IDrawable
{
private:

    OrbitCharacteristics m_characteristics;


    DEFAULT_FIELDS_FOR_DRAWABLE(orbitLine);

public:
    OrbitDrawable(const OrbitCharacteristics &characteristics);

    void draw(const DrawData &drawData)override;
    
    ~OrbitDrawable();
};

