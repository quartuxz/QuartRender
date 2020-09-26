//DEPRECATED

#pragma once
#include <queue>
#include "IDrawable.h"

template<class variationType>
class IVariableDrawable: public IDrawable
{
public:
	typedef variationType variation_t;
protected:
	std::queue<variation_t> m_variations;
public:

	//if the only variation type is
	virtual void addVariation(const variation_t &variation) {
		m_variations.push(variation);
	}

	virtual ~IVariableDrawable() {

	}
};

//if the variation type is a glm::mat4 then we simply dont define an m_variations
//and use IDrawable's m_modelTransformQueue instead
template<>
class IVariableDrawable<glm::mat4> :public IDrawable {
public:
	typedef glm::mat4 variation_t;
	virtual void addVariation(const variation_t &variation) {
		m_modelTransformQueue.push(variation);
	}
	virtual ~IVariableDrawable() {

	}
};

