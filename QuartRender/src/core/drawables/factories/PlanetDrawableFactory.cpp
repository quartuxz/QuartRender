#include "PlanetDrawableFactory.h"

PlanetDrawable* PlanetDrawableFactory::m_getNewDrawable()
{
    return new PlanetDrawable(m_planetCaracteristics);
}

PlanetDrawableFactory::PlanetDrawableFactory(const PlanetCharacteristics &planetCaracteristics):
    m_planetCaracteristics(planetCaracteristics)
{

}
