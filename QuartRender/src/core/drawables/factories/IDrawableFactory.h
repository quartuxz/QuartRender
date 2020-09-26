#pragma once
#include <map>
#include "../../utilsGL.h"

#include "glm/glm.hpp"
#include "../../renderers/RendererThreadManager.h"

/// <summary>
/// the base interface for all drawable factories. They are used to create drawables as illustrated below:
///say we want to draw planet earth on the screen,
///we would create a child of IDrawable called PlanetDrawable, then create a factory called PlanetDrawableFactory,
///PlanetDrawableFactory is solely responsible for creating and managing PlanetDrawable instances with the given parameters,
///1.)to create earth we would create a PlanetDrawableFactory instance with all the parameters characteristic of earth such as
///2.)radius, atmospheric pressure, average color, liquid coverage etc,
///when we want to draw earth we would simply tell the factory to add a PlanetDrawable for drawing to the specified renderer,
///this PlanetDrawable is sometimes created, often reused(per renderer),
///but is always made with and mantains the caracteristics parameters specified in 1.) and 2.)
///this is also when we specify the "variation", this indicates where and how(model transform++) to draw earth,
///(this last thing is done in the "addDrawableToRenderer" function)
///and any other custom things we specify
/// </summary>
class IDrawableFactory
{
protected:

	std::map<uniqueID, IDrawable*> m_testDrawablesPerRenderer;


	/// <summary>
	/// should be defined publicly in the concrete child class, used then to instanciate a DrawableFactory,
	/// for the given parameters desired in created drawables.
	/// </summary>
	/// <returns></returns>
	IDrawableFactory();

	/// <summary>
	/// this function must be overriden to create a drawable of with the desired constructor parameters,
	/// it is called everytime a new object with the given parameters is created to be rendered in a new
	/// renderer
	/// </summary>
	/// <returns>a dynamic object of the factories drawable type</returns>
	virtual IDrawable* m_getNewDrawable() = 0;
public:

	/// <summary>
	///	both creates a new drawable instance with the parameters indicated for this factory instance(creating it on the rendering thread)
	///	or reuses an existing one.
	/// does this to ultimately draw any of them on the given renderer with the given variation
	/// NOTE: causes crashes if called using a destoyed renderer
	/// </summary>
	/// <param name="renderer">The renderer to which the drawable should be displayed.</param>
	/// <param name="variation">The variation(e.g position on screen or/and other specific per-draw info), \
	/// it is worth noting that you can add the same drawable per frame using this same factory, and change \
	/// the variation to achieve a different rendering result(e.g same geometry but different position on screen)</param>
	virtual void addDrawableToRenderer(RendererThreadManager* renderer, const DrawVariation& variation);
	/// <summary>
	/// destroys an instance of the drawable this factory instance is indicated how to create per
	/// renderer instance
	/// </summary>
	/// <param name="renderer">the renderer instance that "owns" a given drawable instance</param>
	virtual void destroyDrawableForRenderer(RendererThreadManager* renderer); 

	virtual ~IDrawableFactory();
};

