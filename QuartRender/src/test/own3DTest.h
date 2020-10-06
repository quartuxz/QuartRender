#include <stdexcept>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../core/utilsGL.h"
#include "../core/VertexArray.h"
#include "../core/IndexBuffer.h"
#include "../core/ShaderProgram.h"


inline void own3DTest() {

	if (!glfwInit())
		throw std::runtime_error("failed to initialize glfw!");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = nullptr;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(500, 500, "...", NULL, NULL);
    if (window)
    {
        glfwTerminate();
        throw std::runtime_error("failed to open window!");
    }



    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("failed to initialize glew!");
    }

    glfwSwapInterval(0);


    //TODO: maybe these hints should be in IRendererer? idk
    THROW_ERRORS_GL(glEnable(GL_BLEND));
    THROW_ERRORS_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    THROW_ERRORS_GL(glEnable(GL_DEPTH_TEST));
    THROW_ERRORS_GL(glDepthFunc(GL_LESS));

    //CLEAR BUFFERS TO TEST COLOR:
    //THROW_ERRORS_GL(glClearColor(0.7f, 0.5f, 0.5f, 1.0f));

    int fbsizex = 0, fbsizey = 0;
    glfwGetFramebufferSize(window, &fbsizex, &fbsizey);
    THROW_ERRORS_GL(glViewport(0, 0, fbsizex, fbsizey));
    
}