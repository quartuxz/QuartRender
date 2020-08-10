#include "TestDrawableFactory.h"


TestDrawable* TestDrawableFactory::m_getNewDrawable()
{
	return new TestDrawable();
}

TestDrawableFactory::TestDrawableFactory()
{
}

TestDrawableFactory::~TestDrawableFactory()
{
}
