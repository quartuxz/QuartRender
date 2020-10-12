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
	GLuint m_geometryShaderID = 0;

	GLuint m_shaderProgramID;

	void m_createShaderProgram(bool usesGeometryShader = false);


public:
	ShaderProgram(const std::optional<std::string> &vertexShader = std::nullopt, const std::optional<std::string>&fragmentShader = std::nullopt, const std::optional<std::string>&geometryShader = std::nullopt);

	//TAKES CARE OF DELETING THE POINTER RETURNED(DELETION HAPPENS WHEN THIS SHADER PROGRAM CALLS ITS DESTRUCTOR!)
	template<UniformTypes uniformType, bool throwNotFound = true>
	UniformGL<uniformType> *getUniformHandle(std::string uniformName) {
		THROW_ERRORS_GL(GLint uniformID = glGetUniformLocation(m_shaderProgramID, uniformName.c_str()));
		//error throwing if the uniform name was not found in the program(all shaders, e.g vertex, fragment etc)
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

//UniformGL SPECIALIZATIONS HERE:

template<>
class UniformGL<UniformTypes::FLOAT_4> :public IUniformGL
{
	friend class ShaderProgram;
public:
	using IUniformGL::IUniformGL;
	void setUniform(GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4)const {
		m_shaderProgram->bind();
		THROW_ERRORS_GL_FAST(glUniform4f(m_uniformID, v1, v2, v3, v4));
	}
};

template<>
class UniformGL<UniformTypes::INT_1> :public IUniformGL
{
	friend class ShaderProgram;
public:
	using IUniformGL::IUniformGL;
	void setUniform(GLint i)const {
		m_shaderProgram->bind();
		THROW_ERRORS_GL_FAST(glUniform1i(m_uniformID, i));
	}
};


template<>
class UniformGL<UniformTypes::FLOAT_MAT_4x4> :public IUniformGL
{
	friend class ShaderProgram;
public:
	using IUniformGL::IUniformGL;
	//note, we calculate everything with 64 bit double containing matrices, but we send the final
	//matrix converted to a 32 bit float to opengl for use in uniforms.
	//this is because we dont use 64-bit-per-element matrices in our openGL shader programs.
	//someday we may.
	//this means the 64-bit-per-element matrix gets converted to its 32 bit version implicitly when calling this
	//function to pass it to.
	void setUniform(const glm::mat4& mat)const {
		m_shaderProgram->bind();
		THROW_ERRORS_GL_FAST(glUniformMatrix4fv(m_uniformID, 1,GL_FALSE,&mat[0][0]));
	}
};


typedef UniformGL<UniformTypes::FLOAT_MAT_4x4> u_MVP_t;