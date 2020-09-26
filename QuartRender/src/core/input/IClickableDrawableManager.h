#pragma once
#include <string>

class IClickableDrawableManager
{
public:
	virtual std::string queryIfHover(double posx, double posy) = 0;
};

