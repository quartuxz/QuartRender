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

int quartRenderFunc(initQuartRender)()
{
    try {
        theDrawableFactoryManager = new DrawableFactoryManager();
    }
    catch (...) {
        return ERROR_TERMINATE_CODE_GL;
    }
   
    return SUCCESS_TERMINATE_CODE_GL;
}
ErrorLogHandle quartRenderFunc(createLogger)()
{
    return new ErrorLog();
}

//TODO: invalid argument detection
RendererHandle quartRenderFunc(createRenderer)(ErrorLogHandle errorLog, unsigned int startx, unsigned int starty, unsigned int rendererType)
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

int quartRenderFunc(renderImage)(RendererHandle renderer, ErrorLogHandle errorLog)
{
    LOG_TO_CONSOLE_COND("render image started.");
    CATCH_LOG_RETURN_GL(theDrawableFactoryManager->endFrame(); GET_RENDERER_MULT(renderer)->display();, GET_ERROR_LOG(errorLog));
    return SUCCESS_TERMINATE_CODE_GL;
}

int quartRenderFunc(getAndAllowClose)(RendererHandle renderer, ErrorLogHandle errorLog, bool* val)
{
    CATCH_LOG_RETURN_GL(
    *val = GET_RENDERER_MULT(renderer)->getAndAllowClose();,
        GET_ERROR_LOG(errorLog)
    )
    return SUCCESS_TERMINATE_CODE_GL;
}

QUARTRENDER_API int quartRenderFunc(createPlanet)(ErrorLogHandle errorLog, const char* planetClassName, PlanetCharacteristics planetCharacteristics)
{
    CATCH_LOG_RETURN_GL(
    PlanetDrawableFactory* planetFac = new PlanetDrawableFactory(planetCharacteristics);

    theDrawableFactoryManager->addClickableDrawableFactory(planetClassName, planetFac);
    , GET_ERROR_LOG(errorLog))
    return SUCCESS_TERMINATE_CODE_GL;
}

int quartRenderFunc(drawPlanet)(RendererHandle renderer, ErrorLogHandle errorLog, const char* planetClassName, const char* planetName, double posx, double posy)
{
    CATCH_LOG_RETURN_GL(
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(posx, posy, 0.0f));
    //we set the union to use the model transform part
    DrawVariation variation;
    variation.modelTransform = transform;
    theDrawableFactoryManager->draw(GET_RENDERER_MULT(renderer), planetClassName, planetName, variation);
    , GET_ERROR_LOG(errorLog))
    return SUCCESS_TERMINATE_CODE_GL;
}

void quartRenderFunc(getRenderImage)(RendererHandle renderer, const std::uint8_t** imgbuf, unsigned int* sizex, unsigned int* sizey)
{
    LOG_TO_CONSOLE_COND("getRenderImage called.");
    auto rendererInstance = GET_RENDERER_MULT(renderer);
    auto temp = rendererInstance->getImageBuffer();
    *sizex = rendererInstance->getViewportWidth();
    *sizey = rendererInstance->getViewportHeight();
    *imgbuf = temp->data();
    
}

void quartRenderFunc(getLogString)(ErrorLogHandle errorLog, char* str, unsigned int *len)
{
    std::string logString = GET_ERROR_LOG(errorLog)->getLog();

    if (str == nullptr) {
        *len = logString.size()+1;
    }
    else {
        strcpy_s(str,*len,logString.c_str());
    }
}


int quartRenderFunc(destroyRenderer)(RendererHandle renderer)
{
    theDrawableFactoryManager->destroyDrawablesForRenderer(GET_RENDERER_MULT(renderer));
    delete GET_RENDERER_MULT(renderer);
    return SUCCESS_TERMINATE_CODE_GL;
}
int quartRenderFunc(destroyLogger)(ErrorLogHandle errorLog)
{
    delete GET_ERROR_LOG(errorLog);
    return SUCCESS_TERMINATE_CODE_GL;
}
int quartRenderFunc(exitQuartRender)()
{
    delete theDrawableFactoryManager;
    glfwTerminate();
    InputManager::destroyAllInputManagers();
    return SUCCESS_TERMINATE_CODE_GL;
}
void quartRenderFunc(getGLVersion)(char*)
{
	glGetString(GL_VERSION);
}

int quartRenderFunc(getAndPopLastKeyboardInput)(RendererHandle renderer, ErrorLogHandle errorLog, KeyboardInput* keyboardInput)
{
    CATCH_LOG_RETURN_GL(*keyboardInput = GET_RENDERER_MULT(renderer)->getInputManager()->getAndPopOldestKeyboardInput();,
        GET_ERROR_LOG(errorLog))
    return SUCCESS_TERMINATE_CODE_GL;
}

int quartRenderFunc(getAndPopLastMouseButtonInput)(RendererHandle renderer, ErrorLogHandle errorLog, MouseButtonInput* mouseButtonInput)
{
    CATCH_LOG_RETURN_GL(*mouseButtonInput = GET_RENDERER_MULT(renderer)->getInputManager()->getAndPopOldestMouseButtonInput();,
        GET_ERROR_LOG(errorLog))
    return SUCCESS_TERMINATE_CODE_GL;
}

int quartRenderFunc(getAndPopLastScrollInput)(RendererHandle renderer, ErrorLogHandle errorLog, ScrollInput* scrollInput)
{
    CATCH_LOG_RETURN_GL(*scrollInput = GET_RENDERER_MULT(renderer)->getInputManager()->getAndPopOldestScrollInput();,
        GET_ERROR_LOG(errorLog))
    return SUCCESS_TERMINATE_CODE_GL;
}

int quartRenderFunc(getCurrentCursorPosition)(RendererHandle renderer, ErrorLogHandle errorLog, CursorPosition* cursorPosition)
{
    CATCH_LOG_RETURN_GL(*cursorPosition = GET_RENDERER_MULT(renderer)->getInputManager()->getCurrentCursorPosition();,
        GET_ERROR_LOG(errorLog))
    return SUCCESS_TERMINATE_CODE_GL;
}



