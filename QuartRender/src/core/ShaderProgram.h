#pragma once
#include <optional>
#include <string>
#include <optional>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "utilsGL.h"

class ShaderProgram;


class IUniformGL {
friend class ShaderProgram;
protected:
	GLuint m_uniformID;
	std::string m_uniformName;

	const ShaderProgram *m_shaderProgram;

	IUniformGL(GLuint uniformID, std::string uniformName, const ShaderProgram *shaderProgram)noexcept;
public:

	IUniformGL(const IUniformGL&)noexcept = delete;
	IUniformGL &operator=(const IUniformGL&)noexcept = delete;
	virtual ~IUniformGL()noexcept;
};


enum class UniformTypes:unsigned int {
	INT_1, FLOAT_4, FLOAT_MAT_4x4, ENUM_MAX
};

//TODO: specialize this class for more and differen uniform types
template<UniformTypes uniformType>
class UniformGL:public IUniformGL {
friend class ShaderProgram;
public:
	using IUniformGL::IUniformGL;
	static_assert(uniformType < UniformTypes::ENUM_MAX, "uniform type not defined!");
};


class ShaderProgram
{
private:
	static constexpr char m_defaultVertexSource[] = "defaultVert.glsl";
	static constexpr char m_defaultFragmentSource[] = "defaultFrag.glsl";


	std::vector<const IUniformGL*> m_uniformHandlers;

	GLuint m_vertexShaderID;
	GLuint m_fragmentShaderID;

	GLuint m_shaderProgramID;

	void m_createShaderProgram();


public:
	ShaderProgram(const std::optional<std::string> &vertexShader = std::nullopt, const std::optional<std::string>&fragmentShader = std::nullopt);

	//TAKES CARE OF DELETING THE POINTER RETURNED(DELETION HAPPENS WHEN THE PARENT SHADER PROGRAM CALL ITS DESTRUCTOR!)
	template<UniformTypes uniformType, bool throwNotFound = true>
	UniformGL<uniformType> *getUniformHandler(std::string uniformName) {
		THROW_ERRORS_GL(GLint uniformID = glGetUniformLocation(m_shaderProgramID, uniformName.c_str()));
		if constexpr(throwNotFound) {
			if (uniformID == -1) {
				std::string errorMsg("Uniform not found!: ");
				errorMsg.append(uniformName);
				throw std::runtime_error(errorMsg);
			}
		}
		UniformGL<uniformType> *retVal = new UniformGL<uniformType>(uniformID, uniformName, this);
		m_uniformHandlers.push_back(retVal);
		return retVal;
	}


	void bind()const;
	void unbind()const;

	~ShaderProgram();
};

//SPECIALIZATIONS HERE:

template<>
class UniformGL<UniformTypes::FLOAT_4> :public IUniformGL {
	friend class ShaderProgram;
public:
	using IUniformGL::IUniformGL;
	void setUniform(GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4)const {
		m_shaderProgram->bind();
		THROW_ERRORS_GL_FAST(glUniform4f(m_uniformID, v1, v2, v3, v4));
	}
};

template<>
class UniformGL<UniformTypes::INT_1> :public IUniformGL {
	friend class ShaderProgram;
public:
	using IUniformGL::IUniformGL;
	void setUniform(GLint i)const {
		m_shaderProgram->bind();
		THROW_ERRORS_GL_FAST(glUniform1i(m_uniformID, i));
	}
};


template<>
class UniformGL<UniformTypes::FLOAT_MAT_4x4> :public IUniformGL {
	friend class ShaderProgram;
public:
	using IUniformGL::IUniformGL;
	void setUniform(const glm::mat4& mat)const {
		m_shaderProgram->bind();
		THROW_ERRORS_GL_FAST(glUniformMatrix4fv(m_uniformID, 1,GL_FALSE,&mat[0][0]));
	}
};