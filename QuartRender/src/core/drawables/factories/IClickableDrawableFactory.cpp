#include "IClickableDrawableFactory.h"

void IClickableDrawableFactory::addDrawableToRenderer(RendererThreadManager* renderer, const DrawVariation& variation, std::string drawableInstanceName)
{
    IDrawableFactory::addDrawableToRenderer(renderer, variation);
}

void IClickableDrawableFactory::endFrame()
{
}

IClickableDrawableFactory::~IClickableDrawableFactory()
{
}

IClickableDrawableFactory::IClickableDrawableFactory()
{
}



