#pragma once
#include <string>
#include "GL/glew.h"

#include "utilsGL.h"

class Texture
{
private:
	GLuint m_textureID;
	std::string m_filePath;
	void* m_localBuffer = nullptr;
	int m_width, m_height, m_bytesPerPixel;
public:
	Texture(const std::string &filePath);
	~Texture();

	void bind(GLuint slot = 0)const;
	void unbind()const;
};

