#pragma once
#include "QuartRender.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "src/core/ErrorLog.h"
#include "src/core/utilsGL.h"
#include "src/core/renderers/RendererThreadManager.h"

#include "src/core/app.cpp"

#include "src/test/TestDrawable.h"

#include "src/test/TestDrawableFactory.h"
#include "src/test/IUniquelyIdentifiableTest.h"


static const char* teststr = "hello";

static RendererThreadManager *testRenderer = nullptr;

static std::vector<TestDrawable*> testDrawables;

static TestDrawableFactory fac;


#define GET_RENDERER_MULT(rendererHandle) static_cast<RendererThreadManager*>(rendererHandle)
#define GET_ERROR_LOG(errorLogHandle) static_cast<ErrorLog*>(errorLogHandle)


const char* runTests()
{

    static std::string testThatFailed = "";
    if (testThatFailed != "") {
        return "ERROR: runTests called more than once!";
    }
    testThatFailed = "once";
    if (!runIUniquelyIdentifiableTest(testThatFailed)) {
        return testThatFailed.c_str();
    }

    return "passed";
}

int drawTest(RendererHandle renderer, ErrorLogHandle errorLog, char* testDrawableName, float posx, float posy)
{
    LOG_TO_CONSOLE_COND("draw test called.");
    CATCH_LOG_RETURN_GL(
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(posx,posy,0.0f));
        fac.addDrawableToRenderer(GET_RENDERER_MULT(renderer), transform);
         ,
    GET_ERROR_LOG(errorLog)
    );
    return SUCCESS_TERMINATE_CODE_GL;
}

int destroyAllDrawTests()
{

    return SUCCESS_TERMINATE_CODE_GL;
}

int addTestError(ErrorLogHandle errorLog)
{
    GET_ERROR_LOG(errorLog)->log(std::runtime_error("HELLO MY FRIEND!!:D"));
    return SUCCESS_TERMINATE_CODE_GL;
}



int startTestRenderer(unsigned int sizex, unsigned int sizey)
{
    try {
        testRenderer = new RendererThreadManager(sizex,sizey,RendererTypes::onscreenRenderer);
    }
    catch (const std::exception&e) {
        std::cerr << e.what() << std::endl;
        return ERROR_TERMINATE_CODE_GL;
    }
    return SUCCESS_TERMINATE_CODE_GL;
}

int renderImageTest(const std::uint8_t** imgbuf, unsigned int* sizex, unsigned int* sizey)
{
    try{
        TestDrawable test;
        LOG_TO_CONSOLE("testDrawable created");
        testRenderer->addDrawable(&test);
        LOG_TO_CONSOLE("testDrawable added");
        testRenderer->display();
        LOG_TO_CONSOLE("image rendered to buffer");
        auto retval = testRenderer->getImageBuffer();
        LOG_TO_CONSOLE("image gotten");
        *sizex = testRenderer->getViewportWidth();
        *sizey = testRenderer->getViewportHeight();
        *imgbuf = retval->data();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return ERROR_TERMINATE_CODE_GL;
    }
    return SUCCESS_TERMINATE_CODE_GL;
}

int stopTestRenderer()
{
    delete testRenderer;
    return SUCCESS_TERMINATE_CODE_GL;
}

int testFunc(const char* in)
{
	int count = 0;
	while (in[count] != '\0')
	{
		count++;
	}

	return count;
}
int initQuartRender()
{

    return SUCCESS_TERMINATE_CODE_GL;
}
ErrorLogHandle createLogger()
{
    return new ErrorLog();
}

//TODO: invalid argument detection
RendererHandle createRenderer(ErrorLogHandle errorLog, unsigned int startx, unsigned int starty, unsigned int rendererType)
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

int renderImage(RendererHandle renderer, ErrorLogHandle errorLog)
{
    LOG_TO_CONSOLE_COND("render image started.");
    CATCH_LOG_RETURN_GL(GET_RENDERER_MULT(renderer)->display(), GET_ERROR_LOG(errorLog));
    return SUCCESS_TERMINATE_CODE_GL;
}

int getAndAllowClose(RendererHandle renderer, ErrorLogHandle errorLog, bool* val)
{
    CATCH_LOG_RETURN_GL(
    *val = GET_RENDERER_MULT(renderer)->getAndAllowClose();,
        GET_ERROR_LOG(errorLog)
    )
    return SUCCESS_TERMINATE_CODE_GL;
}

void getRenderImage(RendererHandle renderer, const std::uint8_t** imgbuf, unsigned int* sizex, unsigned int* sizey)
{
    LOG_TO_CONSOLE_COND("getRenderImage called.");
    auto rendererInstance = GET_RENDERER_MULT(renderer);
    auto temp = rendererInstance->getImageBuffer();
    *sizex = rendererInstance->getViewportWidth();
    *sizey = rendererInstance->getViewportHeight();
    *imgbuf = temp->data();
    
}

void imageTest(unsigned char** imgbuf, unsigned int* sizex, unsigned int* sizey)
{
    *sizex = 800;
    *sizey = 800;
    *imgbuf = new unsigned char[(*sizex)*(*sizey)*4];
    for (size_t i = 0; i < (*sizex) * (*sizey) * 4; i+=4)
    {
        for (size_t o = 0; o < 4; o++)
        {
            if (o == 3) {
                (*imgbuf)[i + o] = 255;
            }
            else if(o==0) {
                (*imgbuf)[i + o] = 50;
            }
            else {
                (*imgbuf)[i + o] = 255;
            }
            
        }

    }
}

void windowTest()
{
    mainNOT();
}

void testStringFunc(char* out, unsigned int* len)
{
    const char start[] = "testfun";
    if (out == nullptr) {
        *len = strlen(start) + 1;
    }
    else {
        strcpy_s(out, *len, start);
    }
}

void getLogString(ErrorLogHandle errorLog, char* str, unsigned int *len)
{
    std::string logString = GET_ERROR_LOG(errorLog)->getLog();

    if (str == nullptr) {
        *len = logString.size()+1;
    }
    else {
        strcpy_s(str,*len,logString.c_str());
    }
}

int destroyRenderer(RendererHandle renderer)
{

    delete GET_RENDERER_MULT(renderer);
    return SUCCESS_TERMINATE_CODE_GL;
}
int destroyLogger(ErrorLogHandle errorLog)
{
    delete GET_ERROR_LOG(errorLog);
    return SUCCESS_TERMINATE_CODE_GL;
}
int exitQuartRender()
{
    glfwTerminate();
    InputManager::destroyAllInputManagers();
    return SUCCESS_TERMINATE_CODE_GL;
}
void getGLVersion(char*)
{
	glGetString(GL_VERSION);
}


