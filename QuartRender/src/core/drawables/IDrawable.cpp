#include "IDrawable.h"


bool IDrawable::setAndPop()
{
	if (!m_modelTransformQueue.empty()) {
		m_modelTransform = m_modelTransformQueue.front();
		m_modelTransformQueue.pop();
		return true;
	}
	return false;
}

IDrawable::~IDrawable()
{
}
