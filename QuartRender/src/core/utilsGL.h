#pragma once
#include <exception>
#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class ExceptionGL :public std::exception {
private:
    const std::vector<GLenum> m_errorTypes;
    const unsigned int m_errorLine;
    const std::string m_errorFile;
    const std::string m_called;
    std::string m_finalWhat;
public:
    const std::string& getErrorFile()const noexcept {
        return m_errorFile;
    }

    unsigned int getErrorLine()const noexcept {
        return m_errorLine;
    }

    const std::vector<GLenum>& getErrorTypes()const noexcept {
        return m_errorTypes;
    }

    ExceptionGL(const std::vector<GLenum>& errorTypes, const std::string& errorFile, unsigned int errorLine, const std::string& called) :
        m_errorTypes(errorTypes),
        m_errorLine(errorLine),
        m_errorFile(errorFile),
        m_called(called)
    {
        std::stringstream ss;
        ss << "Error in file: " << errorFile << " In line: " << errorLine << "\nInvoked by: " << called << "\n--->\nGL ERROR CODES: " << std::endl;
        for (size_t i = 0; i < errorTypes.size(); i++)
        {
            ss << "\t" << errorTypes[i] << std::endl;
        }
        m_finalWhat = ss.str();
    }

    const char* what()const noexcept override {
        return m_finalWhat.c_str();
    }
};

inline void clearGLErrors() {
    while (glGetError() != GL_NO_ERROR);
}

inline std::vector<GLenum> logGLErrors(bool printToCerr = true) {
    std::vector<GLenum> retVal;
    while (GLenum error = glGetError()) {
        retVal.push_back(error);
        if (printToCerr) {
            std::cerr << "GL ERROR: " << error << std::endl;
        }
    }
    return retVal;
}

#define NO_ERROR_CHECKING 0
#define ERROR_TERMINATE_CODE_GL -1
#define SUCCESS_TERMINATE_CODE_GL 0
#define CATCH_LOG_RETURN_GL(expr, loggingObj) try{expr;}catch(const ExceptionGL&e){loggingObj->log(e); return e.getErrorTypes()[0];}catch(const std::exception&e){loggingObj->log(e); return ERROR_TERMINATE_CODE_GL;}
#define LOG_EVERYWHERE 0


#if NO_ERROR_CHECKING
#define THROW_ERRORS_GL(expr) expr;
#define TERMINATE_MAIN_WITH_ERRORS_GL(expr) expr;
#else
#define THROW_ERRORS_GL(expr) clearGLErrors(); expr; {std::vector<GLenum> macroVar_errorCodes; macroVar_errorCodes = logGLErrors(false); if(!macroVar_errorCodes.empty()){ throw ExceptionGL(macroVar_errorCodes, __FILE__, __LINE__,#expr);}}
#define TERMINATE_MAIN_WITH_ERRORS_GL(expr) try{THROW_ERRORS_GL(expr)}catch(const std::exception &e){std::cerr << e.what() << std::endl; return ERROR_TERMINATE_CODE_GL;}
#endif

#define THROW_ERRORS_GL_FAST(expr) THROW_ERRORS_GL(expr)

#ifdef NDEBUG
#define THROW_ERRORS_GL_DEBUG_ONLY(expr) expr;
#define TERMINATE_MAIN_WITH_ERRORS_GL_DEBUG_ONLY(expr) expr;
#else
#define THROW_ERRORS_GL_DEBUG_ONLY(expr) THROW_ERRORS_GL(expr)
#define TERMINATE_MAIN_WITH_ERRORS_GL_DEBUG_ONLY(expr) TERMINATE_MAIN_WITH_ERRORS_GL(expr)
#endif

#define LOG_TO_CONSOLE(str) std::clog << __FILE__ << ", " << __LINE__ << ": " << str << std::endl

#if LOG_EVERYWHERE
#define LOG_TO_CONSOLE_COND(str) LOG_TO_CONSOLE(str)
#else
#define LOG_TO_CONSOLE_COND(str)
#endif

inline std::string loadFile(const std::string& filename) {

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::string errorMessage("failed to open file! ");

        errorMessage.append(filename);

        throw std::runtime_error(errorMessage);
    }
    std::ostringstream sstr;
    sstr << file.rdbuf();
    return sstr.str();
}


//TODO: add all types
template<GLenum typeGLID>
struct getGLType {

};

template<>
struct getGLType<GL_FLOAT> {
    typedef GLfloat typeGL;
};

template<>
struct getGLType<GL_UNSIGNED_INT> {
    typedef GLuint typeGL;
};

//TODO: add all types
inline size_t getSizeOfGLType(GLenum type)noexcept {
    switch (type)
    {
    case GL_FLOAT:
        return  sizeof(getGLType<GL_FLOAT>::typeGL);
    case GL_UNSIGNED_INT:
        return sizeof(getGLType<GL_UNSIGNED_INT>::typeGL);
    default:
        break;
    }
}


inline void glfw_error_callback(int error, const char* description)
{
    //TODO: implement glfw error handling
    fprintf(stderr, "Error: %s\n", description);
}