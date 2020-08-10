#pragma once
#include <queue>

template<class variationType>
class IVariableDrawable
{
public:
	typedef variationType variation_t;
protected:
	std::queue<variation_t> m_variations;
public:

	virtual void addVariation(const variation_t &variation) {
		m_variations.push(variation);
	}

	virtual ~IVariableDrawable() {

	}
};

