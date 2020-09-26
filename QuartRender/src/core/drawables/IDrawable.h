#pragma once
#include <queue>
#include <optional>
#include "DrawDataStructs.h"
#include "glm/glm.hpp"
#include "DrawableVariationUnion.h"

enum class DrawableTypes:unsigned int {
	drawable2D, drawable3D, drawableZoomHybrid, ENUM_MAX
};



/// <summary>
///drawables work by creating a single instance of them(that becomes the drawable "class" e.g a planet)
///this single instance is an instance of a class derived from IDrawable and is created by a single factory
///that is responsible for managing all instances of a given drawable "class" for given drawable parameters.
///to illustrate with an example:
///say we want to draw planet earth on the screen,
///we would create a child of IDrawable called PlanetDrawable, then create a factory called PlanetDrawableFactory,
///very original I know,
///PlanetDrawableFactory is solely responsible for creating and managing PlanetDrawable instances with the given parameters,
///1.)to create earth we would create a PlanetDrawableFactory instance with all the parameters characteristic of earth such as
///2.)radius, atmospheric pressure, average color, liquid coverage etc,
///when we want to draw earth we would simply tell the factory to add a PlanetDrawable for drawing to the specified renderer,
///this PlanetDrawable is sometimes created, often reused(per renderer),
///but is always made with and mantains the caracteristics parameters specified in 1.) and 2.)
///this is also when we specify the "variation" that is popped throught setAndPop(), this indicates where and how(model transform++) to draw earth,
///and any other custom things we specify when inheriting IVariableDrawable in its template parameter.
///when setAndPop() returns false, it means there are no new variations to be set and the renderer is
///done drawing drawables of that instance
/// </summary>
class IDrawable
{
protected:
	DrawVariation m_drawVariation;
	std::queue<DrawVariation> m_drawVariationQueue;
public:
	//TODO: make function take 1.) view and projection matrices
	//TODO: 2.)light sources
	virtual void draw(const DrawData&) = 0;
	//gets the type of drawable this is(e.g. 2D, 3D etc)
	virtual DrawableTypes getDrawableType()const noexcept = 0;
	//set the required state for each variation by popping the variations and using them per this function call,
	//returns true if there was a variation to pop and therefore a draw call remaining, false otherwise.
	//NOTE: is pure virtual but provides a default definition that can be invoked,
	//this definition simply sets the current drawVariation to the front drawVariation in the drawVariation queue and pops it.
	//when overloading be sure to return true when there are no more needed draw calls with given individual variables.
	virtual bool setAndPop();

	/// <summary>
	/// adds a draw variation that affects specific per-draw-instance(single frame) parameters
	/// of a drawable, can have multiple variations queued up, to draw the same drawable in different
	/// ways(e.g different positions on the screen)
	/// </summary>
	/// <param name="drawVariation">The given draw variation, a union containing all different ones.</param>
	virtual void addVariation(const DrawVariation &drawVariation);
	

	virtual ~IDrawable();
};

