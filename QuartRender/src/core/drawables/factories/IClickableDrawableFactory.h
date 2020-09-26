#pragma once
#include <string>
#include "IDrawableFactory.h"


class IClickableDrawableFactory :
    public IDrawableFactory
{
protected:
    /// <summary>
    /// Protected constructor to prevent instanciation of interface.
    /// expose in child class!
    /// </summary>
    /// <returns></returns>
    IClickableDrawableFactory();

public:
    /// <summary>
    /// same as addDrawableToRenderer in parent IDrawableFactory but adds a unique per drawable instance name to discern who
    /// was clicked on the screen in a given frame
    /// </summary>
    /// <param name="renderer"></param>
    /// <param name="variation"></param>
    /// <param name="drawableInstanceName"></param>
    virtual void addDrawableToRenderer(RendererThreadManager* renderer, const DrawVariation& variation, std::string drawableInstanceName);

    /// <summary>
    /// indicates to the class that the frame ended so it can refresh the cursor position comparisons
    /// </summary>
    virtual void endFrame();

    /// <summary>
    /// virtual destructor to allow polymorphic deletion
    /// </summary>
    /// <returns></returns>
    virtual ~IClickableDrawableFactory();
};

