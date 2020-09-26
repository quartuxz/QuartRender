#pragma once
#include "IClickableDrawableFactory.h"
#include "../drawable classes/PlanetDrawable.h"

class PlanetDrawableFactory final:
    public IClickableDrawableFactory
{
private:
    PlanetDrawable* m_getNewDrawable()override;
    PlanetCharacteristics m_planetCaracteristics;
public:

    PlanetDrawableFactory(const PlanetCharacteristics &planetCaracteristics);
};

