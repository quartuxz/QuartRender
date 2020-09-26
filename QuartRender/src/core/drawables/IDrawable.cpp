#include "IDrawable.h"


bool IDrawable::setAndPop()
{
	if (!m_drawVariationQueue.empty()) {
		m_drawVariation = m_drawVariationQueue.front();
		m_drawVariationQueue.pop();
		return true;
	}
	return false;
}

void IDrawable::addVariation(const DrawVariation &drawVariation)
{
	m_drawVariationQueue.push(drawVariation);
}

IDrawable::~IDrawable()
{
}
