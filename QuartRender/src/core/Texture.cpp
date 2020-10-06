#include "Texture.h"
#include "../vendor/PNGLoader(stb_image)/stb_image.h"

Texture::Texture(const std::string &filePath):
	m_filePath(filePath)
{
	stbi_set_flip_vertically_on_load(1);
	m_localBuffer = stbi_load(filePath.c_str(), &m_width, &m_height, &m_bytesPerPixel, 4);
	if (m_localBuffer == nullptr) {
		throw std::runtime_error("TEXTURE FAILED TO LOAD!");
	}
	THROW_ERRORS_GL(glGenTextures(1, &m_textureID));
	THROW_ERRORS_GL(glBindTexture(GL_TEXTURE_2D, m_textureID));

	THROW_ERRORS_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	THROW_ERRORS_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	THROW_ERRORS_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	THROW_ERRORS_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	THROW_ERRORS_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width,m_height,0,GL_RGBA,GL_UNSIGNED_BYTE,m_localBuffer));
	
	//we unbind the texture
	THROW_ERRORS_GL_FAST(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_localBuffer != nullptr) {
		stbi_image_free(m_localBuffer);
	}
}

Texture::~Texture()
{
	THROW_ERRORS_GL(glDeleteTextures(1,&m_textureID));
}

void Texture::bind(GLuint slot) const
{
	m_boundSlotGL = slot;
	THROW_ERRORS_GL_FAST(glActiveTexture(GL_TEXTURE0+slot));
	THROW_ERRORS_GL_FAST(glBindTexture(GL_TEXTURE_2D,m_textureID));
}

int Texture::getBoundSlotGL() const
{
	return m_boundSlotGL;
}

void Texture::unbind() const
{
	if (m_boundSlotGL != -1) {
		m_boundSlotGL = -1;
		THROW_ERRORS_GL_FAST(glActiveTexture(GL_TEXTURE0+m_boundSlotGL));
		THROW_ERRORS_GL_FAST(glBindTexture(GL_TEXTURE_2D, 0));
	}
}
