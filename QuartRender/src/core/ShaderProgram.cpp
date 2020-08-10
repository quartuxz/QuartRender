#include "ShaderProgram.h"
#include <exception>
#include <sstream>
#include "utilsGL.h"


static GLuint m_compileShader(GLenum type, const std::string& source){

    THROW_ERRORS_GL(GLuint id = glCreateShader(type));
    const char* src = source.c_str();
    THROW_ERRORS_GL(glShaderSource(id, 1, &src, nullptr));
    THROW_ERRORS_GL(glCompileShader(id));

    GLint result;
    THROW_ERRORS_GL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        GLint length;
        THROW_ERRORS_GL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = new char[length];
        THROW_ERRORS_GL(glGetShaderInfoLog(id, length, &length, message));
        THROW_ERRORS_GL(glDeleteShader(id));

        std::stringstream ss;
        ss << "failed to copile shader, shader type: " << type << ", error: " << message;
        throw std::runtime_error(ss.str());

    }

    return id;

}
void ShaderProgram::m_createShaderProgram()
{
    THROW_ERRORS_GL(m_shaderProgramID = glCreateProgram());

    THROW_ERRORS_GL(glAttachShader(m_shaderProgramID, m_vertexShaderID));
    THROW_ERRORS_GL(glAttachShader(m_shaderProgramID, m_fragmentShaderID));
    THROW_ERRORS_GL(glLinkProgram(m_shaderProgramID));
    THROW_ERRORS_GL(glValidateProgram(m_shaderProgramID));

    THROW_ERRORS_GL(glDetachShader(m_shaderProgramID, m_vertexShaderID));
    THROW_ERRORS_GL(glDetachShader(m_shaderProgramID, m_fragmentShaderID));

    THROW_ERRORS_GL(glDeleteShader(m_vertexShaderID));
    THROW_ERRORS_GL(glDeleteShader(m_fragmentShaderID));
}

ShaderProgram::ShaderProgram(const std::optional<std::string>& vertexShader, const std::optional<std::string>& fragmentShader)
{
	std::string vertexSource = m_defaultFragmentSource;
	if (vertexShader.has_value()) {
		vertexSource = vertexShader.value();
	}
	
	std::string fragmentSource = m_defaultFragmentSource;
	if (fragmentShader.has_value()) {
		fragmentSource = fragmentShader.value();
	}

    m_vertexShaderID = m_compileShader(GL_VERTEX_SHADER, loadFile(vertexSource));
    m_fragmentShaderID =m_compileShader(GL_FRAGMENT_SHADER, loadFile(fragmentSource));
    m_createShaderProgram();

}

void ShaderProgram::bind()const
{
    THROW_ERRORS_GL_FAST(glUseProgram(m_shaderProgramID));
}

void ShaderProgram::unbind()const
{

    THROW_ERRORS_GL_FAST(glUseProgram(0));
}

ShaderProgram::~ShaderProgram()
{

    for (auto const&uniformHandler:m_uniformHandlers) {
        delete uniformHandler;
    }

    THROW_ERRORS_GL(glDeleteProgram(m_shaderProgramID));
}

IUniformGL::IUniformGL(GLuint uniformID, std::string uniformName, const ShaderProgram *shaderProgram) noexcept:
    m_uniformID(uniformID),
    m_uniformName(uniformName),
    m_shaderProgram(shaderProgram)
{

}

IUniformGL::~IUniformGL() noexcept
{
}
