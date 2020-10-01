#include "Renderer.h"
#include <stdexcept>
#include "../utilsGL.h"

Renderer::Renderer(unsigned int width, unsigned int height):
    IRenderer(width, height)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    m_window = glfwCreateWindow(1, 1, "Hello World", NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        throw std::runtime_error("failed to open window!");
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_window);

    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("failed to initialize glew!");
    }

    glfwSwapInterval(0);

    setViewportDimensions(width,height);
    m_isInit = true;

}

Renderer::~Renderer()
{
    glfwDestroyWindow(m_window);
}

bool Renderer::windowShouldClose()
{
    return false;
}

void Renderer::setViewportDimensions(unsigned int width, unsigned int height)
{

    if (m_isInit) {
        m_deleteGLBuffers();
    }


    IRenderer::setViewportDimensions(width, height);


    /* Color renderbuffer. */
    THROW_ERRORS_GL(glGenRenderbuffers(1, &rbo_color));
    THROW_ERRORS_GL(glBindRenderbuffer(GL_RENDERBUFFER, rbo_color));
    THROW_ERRORS_GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height));

    /* Depth renderbuffer. */
    THROW_ERRORS_GL(glGenRenderbuffers(1, &rbo_depth));
    THROW_ERRORS_GL(glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth));
    THROW_ERRORS_GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height));

    /*  Framebuffer */
    THROW_ERRORS_GL(glGenFramebuffers(1, &fbo));
    //binds the framebuffer as current "draw target"
    THROW_ERRORS_GL(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
    THROW_ERRORS_GL(glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo_color));
    THROW_ERRORS_GL(glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth));


    THROW_ERRORS_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    THROW_ERRORS_GL(glEnable(GL_BLEND));
    THROW_ERRORS_GL(glEnable(GL_DEPTH_TEST));

    //CLEAR BUFFERS TO TEST COLOR:
    THROW_ERRORS_GL(glClearColor(0.7f,0.5f,0.5f,1.0f));


    THROW_ERRORS_GL(glViewport(0, 0, width, height));

    THROW_ERRORS_GL_FAST(glReadBuffer(GL_COLOR_ATTACHMENT0));
    LOG_TO_CONSOLE_COND("gl read buffer set.");


    //resize to width*height * 4 color channels per pixel RGBA
    m_currentImageBuffer.resize(width*height*4);

}


//TODO: FIX PERFORMANCE PROBLEMS(actually scrap offscreen rendering lmao)
void Renderer::display()
{
    IRenderer::display();
    LOG_TO_CONSOLE_COND("draw queue drawn.");
    THROW_ERRORS_GL_FAST(glReadPixels(0,0,m_width,m_height,GL_RGBA,GL_UNSIGNED_BYTE,m_currentImageBuffer.data()));
    LOG_TO_CONSOLE_COND("gl buffer read.");
}

const std::vector<std::uint8_t>* Renderer::getImageBuffer()const noexcept
{
    return &m_currentImageBuffer;
}

void Renderer::m_draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shaderProgram) const
{
	shaderProgram.bind();
	vertexArray.bind();
	indexBuffer.bind();
	THROW_ERRORS_GL(glDrawElements(GL_TRIANGLES,indexBuffer.getCount(),GL_UNSIGNED_INT,nullptr));
}

void Renderer::m_deleteGLBuffers()
{
    glDeleteFramebuffers(1, &fbo);
    glDeleteRenderbuffers(1, &rbo_color);
    glDeleteRenderbuffers(1, &rbo_depth);
}
