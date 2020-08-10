#include "Texture.h"
#include "../vendor/PNGLoader(stb_image)/stb_image.h"

Texture::Texture(const std::string &filePath):
	m_filePath(filePath)
{
	stbi_set_flip_vertically_on_load(1);
	m_localBuffer = stbi_load(filePath.c_str(), &m_width, &m_height, &m_bytesPerPixel, 4);
	if (m_localBuffer == nullptr) {
		std::cerr << "TEXTURE NOT LOADED!";
	}
	THROW_ERRORS_GL(glGenTextures(1, &m_textureID));
	THROW_ERRORS_GL(glBindTexture(GL_TEXTURE_2D, m_textureID));

	THROW_ERRORS_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	THROW_ERRORS_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	THROW_ERRORS_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	THROW_ERRORS_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	THROW_ERRORS_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width,m_height,0,GL_RGBA,GL_UNSIGNED_BYTE,m_localBuffer));
	unbind();

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
	THROW_ERRORS_GL_FAST(glActiveTexture(GL_TEXTURE0+slot));
	THROW_ERRORS_GL_FAST(glBindTexture(GL_TEXTURE_2D,m_textureID));
}

void Texture::unbind() const
{
	THROW_ERRORS_GL_FAST(glBindTexture(GL_TEXTURE_2D,0));
}
