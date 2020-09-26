#pragma once
#include <map>
#include <string>
#include "factories/IDrawableFactory.h"
#include "factories/IClickableDrawableFactory.h"
#include "../renderers/RendererThreadManager.h"

//TODO:
//add mouse hit detection for certain drawables
//figure out if I should cache all draw calls to wait until e.g all light sources are declared for drawing
//or if I should simply demand they be declared first
class DrawableFactoryManager
{
private:
	std::map<std::string, IDrawableFactory*> m_drawableFactories;
	std::map<std::string, IClickableDrawableFactory*> m_clickableDrawableFactories;
public:
	//TODO: implement this to keep track of when the frame ends and serve for per-frame mouse hit detection
	void endFrame();

	/// <summary>
	/// adds a drawable factory by name to a map. 
	/// the factory is dynamically allocated by the user(with new) and deleted by this class
	/// </summary>
	/// <param name="name">the name of the factory(e.g earth), used to create drawables through it</param>
	/// <param name="factory">the actual factory object, dynamically allocated by the user(with new), this class handles destruction</param>
	void addDrawableFactory(std::string name,IDrawableFactory* factory);

	/// <summary>
	/// adds a clickable drawable factory by name to a map.
	/// these can be queried for wether they lie under a cursor position or not
	/// the factory is dynamically allocated by the user(with new) and deleted by this class
	/// </summary>
	/// <param name="name">the name of the clickable drawable factory(e.g earth but you can query if mouse is over it), used to create clickable drawables through it
	/// that you can query if they are in a given position after drawn</param>
	/// <param name="factory">the actual factory object, dynamically allocated by the user(with new), this class handles destruction</param>
	void addClickableDrawableFactory(std::string name, IClickableDrawableFactory* factory);

	/// <summary>
	// TODO: enable the drawInstanceNames and test hit detection with them.\ 
	/// draws using a given factory and with a given "instance name" to a given renderer,
	/// the "instance name" differentiates between same "drawables" with different draw variations,
	/// this is used for mouse hit detection by returning the name provided in case the detection does detect a hit.
	/// example:
	/// earth is the factory name, it creates earths, an instance of earth simply would specify mainly where(position)
	/// and sometimes how to draw it, that instance must have a name to differentiate among other earth instances,
	/// for example: earth.001
	/// this of course seems silly, theres only one earth, but it allows us to reuse factories and still be able
	/// to tell instances created by them with different draw variations such as position.
	/// </summary>
	/// <param name="renderer">a RendererThreadManager where the drawable is drawn to</param>
	/// <param name="factoryName">the factory that will create/reuse(existing) the drawable for drawing.</param>
	/// <param name="drawInstanceName">the name of the draw instance, explained in summary.</param>
	/// <param name="variation">the draw variation for this draw instance(e.g model matrix and more).</param>
	void draw(RendererThreadManager* renderer, std::string factoryName, std::string drawInstanceName, const DrawVariation& variation);

	/// <summary>
	/// same as draw but assigns no per-draw-instance name to the drawable drawn, disabling from being able to be queried for mouse hovering
	/// prefer this when drawing simple drawables with no such functionality.
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="factoryName"></param>
	/// <param name="variation"></param>
	void drawAnonymous(RendererThreadManager* renderer, std::string factoryName, const DrawVariation& variation);

	/// <summary>
	/// destroys all realted memory to a renderer, effectively unbinding any use of the given renderer pointer
	/// this allows such pointed memory to be safely deleted.
	/// </summary>
	/// <param name="renderer">the renderer pointer to be unbound</param>
	void destroyDrawablesForRenderer(RendererThreadManager *renderer);

	~DrawableFactoryManager();
};

