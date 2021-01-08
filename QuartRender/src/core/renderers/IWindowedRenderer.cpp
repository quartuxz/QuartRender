#include "IWindowedRenderer.h"
#include <stdexcept>

#include "../input/InputManager.h"

#include "../utilsGL.h"

IWindowedRenderer::IWindowedRenderer(unsigned int sizex, unsigned int sizey):
	IRenderer(sizex,sizey)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    m_window = glfwCreateWindow(sizex, sizey, "...", NULL, NULL);
    if (m_window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("failed to open window!");
    }


    InputManager::registerInputManagerInThread(this);

    glfwSetKeyCallback(m_window, key_callback);
    glfwSetMouseButtonCallback(m_window, mouseButton_callback);
    glfwSetScrollCallback(m_window, scroll_callback);
    glfwSetCursorPosCallback(m_window, cursorPosition_callback);
    glfwSetWindowSizeCallback(m_window, windowResize_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(m_window);

    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("failed to initialize glew!");
    }

    glfwSwapInterval(0);


    //TODO: maybe these should be in IRendererer? i   dk
    THROW_ERRORS_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    THROW_ERRORS_GL(glEnable(GL_BLEND));
    THROW_ERRORS_GL(glEnable(GL_DEPTH_TEST));
    THROW_ERRORS_GL(glDepthFunc(GL_LESS));
    THROW_ERRORS_GL(glEnable(GL_CULL_FACE));

    //CLEAR BUFFERS TO TEST COLOR:
    THROW_ERRORS_GL(glClearColor(0.7f, 0.5f, 0.5f, 1.0f));

    int fbsizex = 0, fbsizey = 0;
    glfwGetFramebufferSize(m_window, &fbsizex, &fbsizey);
    THROW_ERRORS_GL(glViewport(0, 0, fbsizex, fbsizey));
}

void IWindowedRenderer::setViewportDimensions(unsigned int sizex, unsigned int sizey)
{
	IRenderer::setViewportDimensions(sizex, sizey);
	glfwSetWindowSize(m_window, sizex, sizey);
}

void IWindowedRenderer::display()
{
    int newvsx = 0, newvsy = 0;
    glfwGetFramebufferSize(m_window, &newvsx, &newvsy);
    THROW_ERRORS_GL_FAST(glViewport(0, 0, newvsx, newvsy));
    IRenderer::display();
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

IWindowedRenderer::~IWindowedRenderer()
{
    InputManager::unregisterInputManagerInThread();
    glfwDestroyWindow(m_window);
}
