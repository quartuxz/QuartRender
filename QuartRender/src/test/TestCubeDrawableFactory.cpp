#include "TestCubeDrawableFactory.h"
#include "TestCubeDrawable.h"


IDrawable* TestCubeDrawableFactory::m_getNewDrawable()
{
    return new TestCubeDrawable();
}

TestCubeDrawableFactory::TestCubeDrawableFactory()
{
}
