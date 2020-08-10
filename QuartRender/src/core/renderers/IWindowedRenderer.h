#pragma once
#include "IRenderer.h"
class IWindowedRenderer :
    public IRenderer
{
protected:
    IWindowedRenderer(unsigned int sizex, unsigned int sizey);
public:
    virtual void setViewportDimensions(unsigned int sizex, unsigned int sizey)override;
    virtual void display()override;

    virtual ~IWindowedRenderer();
};

