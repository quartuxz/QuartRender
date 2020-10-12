#pragma once
#include "QuartRender.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "quartRenderDLLImplUtils.h"

#include "src/core/app.cpp"

#include "src/test/TestDrawable.h"

#include "src/test/TestDrawableFactory.h"
#include "src/test/IUniquelyIdentifiableTest.h"

#include "src/core/drawables/DrawableFactoryManager.h"

#include "src/core/drawables/factories/PlanetDrawableFactory.h"


static DrawableFactoryManager *theDrawableFactoryManager;

int quartRenderFuncName(initQuartRender)()
{
    
    srand(time(NULL));

    try {
        theDrawableFactoryManager = new DrawableFactoryManager();
    }
    catch (...) {
        return ERROR_TERMINATE_CODE_GL;
    }
   
    return SUCCESS_TERMINATE_CODE_GL;
}
ErrorLogHandle quartRenderFuncName(createLogger)()
{
    return new ErrorLog();
}

//TODO: invalid argument detection
RendererHandle quartRenderFuncName(createRenderer)(ErrorLogHandle errorLog, unsigned int startx, unsigned int starty, unsigned int rendererType)
{
    RendererThreadManager* retVal = nullptr;
    try{
        if (rendererType >= unsigned int(RendererTypes::ENUM_MAX)) {
            throw std::invalid_argument("rendererType specified does not exist!");
        }
        retVal = new RendererThreadManager(startx,starty, RendererTypes(rendererType));
    }
    catch (const std::exception& e) {
        GET_ERROR_LOG(errorLog)->log(e);
    }

    return retVal;
}

int quartRenderFuncName(renderImage)(RendererHandle renderer, ErrorLogHandle errorLog)
{
    LOG_TO_CONSOLE_COND("render image started.");
    CATCH_LOG_RETURN_GL(theDrawableFactoryManager->endFrame(); GET_RENDERER_MULT(renderer)->display();, GET_ERROR_LOG(errorLog));
    return SUCCESS_TERMINATE_CODE_GL;
}

int quartRenderFuncName(getAndAllowClose)(RendererHandle renderer, ErrorLogHandle errorLog, bool* val)
{
    CATCH_LOG_RETURN_GL(
    *val = GET_RENDERER_MULT(renderer)->getAndAllowClose();,
        GET_ERROR_LOG(errorLog)
    )
    return SUCCESS_TERMINATE_CODE_GL;
}

uint32_t quartRenderFuncName(getViewportWidth)(RendererHandle renderer)
{
    return GET_RENDERER_MULT(renderer)->getViewportWidth();
}

uint32_t quartRenderFuncName(getViewportHeight)(RendererHandle renderer)
{
    return GET_RENDERER_MULT(renderer)->getViewportHeight();
}

QUARTRENDER_API int quartRenderFuncName(createPlanet)(ErrorLogHandle errorLog, const char* planetClassName, PlanetCharacteristics planetCharacteristics)
{
    CATCH_LOG_RETURN_GL(
    PlanetDrawableFactory* planetFac = new PlanetDrawableFactory(planetCharacteristics);

    theDrawableFactoryManager->addClickableDrawableFactory(planetClassName, planetFac);
    , GET_ERROR_LOG(errorLog))
    return SUCCESS_TERMINATE_CODE_GL;
}

int quartRenderFuncName(drawPlanet)(RendererHandle renderer, ErrorLogHandle errorLog, const char* planetClassName, const char* planetName, double posx, double posy)
{
    CATCH_LOG_RETURN_GL(
        //using two separate transform matrices for future proofing.
        glm::f64mat4 transform2D = glm::translate(glm::f64mat4(1.0), glm::f64vec3(posx, posy, 0.0));
        glm::f64mat4 transform3D = glm::translate(glm::f64mat4(1.0),glm::f64vec3(posx,posy,0.0));
        //rotate the sphere 90 degrees
        transform3D = transform3D * glm::rotate(glm::f64mat4(1.0), glm::radians(90.0), glm::f64vec3(1.0,0.0,0.0));
        //we set the union to use the model transform part
        DrawVariation variation;
        variation.modelTransform2D3DHybrid.modelTransform2D = transform2D;
        variation.modelTransform2D3DHybrid.modelTransform3D = transform3D;
        theDrawableFactoryManager->draw(GET_RENDERER_MULT(renderer), planetClassName, planetName, variation);
    , GET_ERROR_LOG(errorLog))
    return SUCCESS_TERMINATE_CODE_GL;
}

int quartRenderFuncName(zoom)(RendererHandle renderer, ErrorLogHandle errorLog, double delta)
{
    CATCH_LOG_RETURN_GL(GET_RENDERER_MULT(renderer)->getDrawDataRef().zoom(delta);,
        GET_ERROR_LOG(errorLog));
    return SUCCESS_TERMINATE_CODE_GL;
}

QUARTRENDER_API int quartRenderFuncName(setZoom)(RendererHandle renderer, ErrorLogHandle errorLog, double value)
{
    CATCH_LOG_RETURN_GL(
    GET_RENDERER_MULT(renderer)->getDrawDataRef().setZoom(value);,
        GET_ERROR_LOG(errorLog))
    return SUCCESS_TERMINATE_CODE_GL;
}

double quartRenderFuncName(getZoom)(RendererHandle renderer)
{
    return GET_RENDERER_MULT(renderer)->getDrawDataRef().getZoom();
}

int quartRenderFuncName(displace)(RendererHandle renderer, ErrorLogHandle errorLog, double xdisplace, double ydisplace, double zdisplace)
{
    CATCH_LOG_RETURN_GL(GET_RENDERER_MULT(renderer)->getDrawDataRef().displace(glm::f64vec3(xdisplace, ydisplace, zdisplace));,
        GET_ERROR_LOG(errorLog));
    return SUCCESS_TERMINATE_CODE_GL;
}

void quartRenderFuncName(getRenderImage)(RendererHandle renderer, const std::uint8_t** imgbuf, unsigned int* sizex, unsigned int* sizey)
{
    LOG_TO_CONSOLE_COND("getRenderImage called.");
    auto rendererInstance = GET_RENDERER_MULT(renderer);
    auto temp = rendererInstance->getImageBuffer();
    *sizex = rendererInstance->getViewportWidth();
    *sizey = rendererInstance->getViewportHeight();
    *imgbuf = temp->data();
    
}

void quartRenderFuncName(getLogString)(ErrorLogHandle errorLog, char* str, unsigned int *len)
{
    std::string logString = GET_ERROR_LOG(errorLog)->getLog();

    if (str == nullptr) {
        *len = logString.size()+1;
    }
    else {
        strcpy_s(str,*len,logString.c_str());
    }
}


int quartRenderFuncName(destroyRenderer)(RendererHandle renderer)
{
    theDrawableFactoryManager->destroyDrawablesForRenderer(GET_RENDERER_MULT(renderer));
    delete GET_RENDERER_MULT(renderer);
    return SUCCESS_TERMINATE_CODE_GL;
}
int quartRenderFuncName(destroyLogger)(ErrorLogHandle errorLog)
{
    delete GET_ERROR_LOG(errorLog);
    return SUCCESS_TERMINATE_CODE_GL;
}
int quartRenderFuncName(exitQuartRender)()
{
    delete theDrawableFactoryManager;
    glfwTerminate();
    InputManager::destroyAllInputManagers();
    return SUCCESS_TERMINATE_CODE_GL;
}
void quartRenderFuncName(getGLVersion)(char*)
{
	glGetString(GL_VERSION);
}

int quartRenderFuncName(getAndPopLastKeyboardInput)(RendererHandle renderer, ErrorLogHandle errorLog, KeyboardInput* keyboardInput)
{
    CATCH_LOG_RETURN_GL(*keyboardInput = GET_RENDERER_MULT(renderer)->getInputManager()->getAndPopOldestKeyboardInput();,
        GET_ERROR_LOG(errorLog))
    return SUCCESS_TERMINATE_CODE_GL;
}

int quartRenderFuncName(getAndPopLastMouseButtonInput)(RendererHandle renderer, ErrorLogHandle errorLog, MouseButtonInput* mouseButtonInput)
{
    CATCH_LOG_RETURN_GL(*mouseButtonInput = GET_RENDERER_MULT(renderer)->getInputManager()->getAndPopOldestMouseButtonInput();,
        GET_ERROR_LOG(errorLog))
    return SUCCESS_TERMINATE_CODE_GL;
}

int quartRenderFuncName(getAndPopLastScrollInput)(RendererHandle renderer, ErrorLogHandle errorLog, ScrollInput* scrollInput)
{
    CATCH_LOG_RETURN_GL(*scrollInput = GET_RENDERER_MULT(renderer)->getInputManager()->getAndPopOldestScrollInput();,
        GET_ERROR_LOG(errorLog))
    return SUCCESS_TERMINATE_CODE_GL;
}

int quartRenderFuncName(getCurrentCursorPosition)(RendererHandle renderer, ErrorLogHandle errorLog, CursorPosition* cursorPosition)
{
    CATCH_LOG_RETURN_GL(*cursorPosition = GET_RENDERER_MULT(renderer)->getInputManager()->getCurrentCursorPosition();,
        GET_ERROR_LOG(errorLog))
    return SUCCESS_TERMINATE_CODE_GL;
}



