#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <exception>
#include <assert.h>
#include <chrono>
#include <cmath>

#include "glm/mat4x4.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "utilsGL.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

static void runAll() {
    {
        std::chrono::steady_clock clock;
        std::chrono::time_point lastTime = clock.now();
        std::chrono::duration<double> delta = std::chrono::duration<double>::zero();

        GLFWwindow* window;

        /* Initialize the library */
        if (!glfwInit())
            throw std::runtime_error("failed to initialize glfw");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            throw std::runtime_error("failed to open window!");
        }


        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        glfwSwapInterval(1);

        if (glewInit() != GLEW_OK) {
            std::cerr << "GLEWINIT failed!" << std::endl;
        }


        THROW_ERRORS_GL(std::clog << glGetString(GL_VERSION) << std::endl);
        THROW_ERRORS_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        THROW_ERRORS_GL(glEnable(GL_BLEND));
        THROW_ERRORS_GL(glEnable(GL_DEPTH_TEST));

        GLfloat positions[] = {
            -0.5f ,-0.5f, 0.0f, 0.0f,
            0.5f ,-0.5f, 1.0f, 0.0f,
            0.5f ,0.5f, 1.0f,1.0f,
            -0.5f ,0.5f, 0.0f,1.0f
        };

        GLuint indices[] = {
            0,1,2,
            2,3,0
        };



        VertexArray vertexArray;
        VertexBuffer vertexBuffer(positions, 4 * 4 * sizeof(GLfloat));
        BufferLayout layout;
        layout.addBufferLayoutElement(BufferLayoutElement(2,GL_FLOAT,GL_FALSE));
        layout.addBufferLayoutElement(BufferLayoutElement(2,GL_FLOAT,GL_FALSE));
        vertexArray.addBuffer(&vertexBuffer,layout);



        IndexBuffer indexBuffer(indices, 6);

        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f,1.5f,-1.0f,1.0f);
        //glm::mat4 proj = glm::perspective(75.0f, 1.0f,0.1f,10000.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.f,0.f,0.f))*glm::scale(glm::mat4(1.0f),glm::vec3(1,1,1));
        glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(0.f,0.f,0.f));


        glm::mat4 mvp = proj * view*model;

        ShaderProgram program("./render_res/shaders/testVert.glsl", "./render_res/shaders/testFrag.glsl");

        auto u_color = program.getUniformHandle<UniformTypes::FLOAT_4>("u_color");
        u_color->setUniform(0.5f,0.5f,0.7f,1.0f);
        auto u_texture = program.getUniformHandle<UniformTypes::INT_1>("u_tex");
        u_texture->setUniform(0);
        auto u_MVP = program.getUniformHandle<UniformTypes::FLOAT_MAT_4x4>("u_MVP");
        u_MVP->setUniform(mvp);

        Texture texture("./render_res/tex/cryo_area_1.png");
        texture.bind();

        //resetting all binds for testing:
        THROW_ERRORS_GL(glBindVertexArray(0));
        THROW_ERRORS_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
        THROW_ERRORS_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        THROW_ERRORS_GL(glUseProgram(0));

        GLfloat animRed = 0;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            delta = std::chrono::duration_cast<std::chrono::milliseconds>(clock.now() - lastTime);
            lastTime = clock.now();

            //std::clog << delta.count() << std::endl;
            animRed += GLfloat(delta.count());
            animRed -= std::trunc(animRed);



            /* Render here */
            //rebinding for drawing
            THROW_ERRORS_GL_FAST(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


            //THROW_ERRORS_GL(glViewport(0,0,600*animRed,600*animRed));

            u_color->setUniform(animRed, 0.3f, 0.1f, 1.0f);
            


            vertexArray.bind();
            program.bind();
            indexBuffer.bind();
            THROW_ERRORS_GL(glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr));


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        //THROW_ERRORS_GL(glDeleteProgram(shaderProgram));
    }
    glfwTerminate();

}


static int mainNOT()
{
    try {
        runAll();

    }
    catch(std::exception &e){
        std::cout << e.what() << std::endl;
        return ERROR_TERMINATE_CODE_GL;
    }
    return 0;
}