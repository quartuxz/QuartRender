#pragma once
#include <string>
#include "GL/glew.h"

#include "utilsGL.h"

class Texture
{
private:
	GLuint m_textureID;
	std::string m_filePath;


	//TODO: may want to change this a queue for each used texture slot
	mutable int m_boundSlotGL = -1;

	void* m_localBuffer = nullptr;
	int m_width, m_height, m_bytesPerPixel;
public:
	Texture(const std::string &filePath);
	~Texture();

	void bind(GLuint slot = 0)const;

	
	/// <summary>
	/// get the current texture slot that was used when binding this texture.
	/// </summary>
	/// <returns>the currently used texture slot, -1 if not using any</returns>
	int getBoundSlotGL()const;

	void unbind()const;
};

