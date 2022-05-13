#include "Graphics/GLTexture.h"
#include <GLEW/glew.h> //This means that this file should not be compiled before GLRenderer.cpp
#include <iostream>
#include <stb_image/stb_image.h>

using namespace SIN;

GLTexture::~GLTexture()
{
	glDeleteTextures(1, &m_BufferID);
}

void GLTexture::LoadResource(const std::string& a_FilePath, bool a_StorePixelBuffer)
{
	stbi_set_flip_vertically_on_load(true);

	unsigned char* buffer = stbi_load(a_FilePath.c_str(), &m_Width, &m_Height, &m_Bitdepth, 4);
	if (buffer == NULL) {
		std::cout << "ERROR.... Could not find texture at path: " << a_FilePath << std::endl;
		return;
	}

	if (a_StorePixelBuffer) {
		m_PixelBuffer = new PixelBuffer();
		m_PixelBuffer->width = m_Width;
		m_PixelBuffer->height = m_Height;
		m_PixelBuffer->bitdepth = m_Bitdepth;
		m_PixelBuffer->data = buffer;
	}

	GenOpenGLTexture(buffer);

	if (!a_StorePixelBuffer)
		stbi_image_free(buffer);
}

void GLTexture::LoadFromPixelBuffer(PixelBuffer* a_PixelBuffer)
{
	m_PixelBuffer = a_PixelBuffer;
	m_Width = a_PixelBuffer->width;
	m_Height = a_PixelBuffer->height;
	m_Bitdepth = a_PixelBuffer->bitdepth;
	GenOpenGLTexture(a_PixelBuffer->data);
}

void GLTexture::GenOpenGLTexture(unsigned char* buffer)
{
	glGenTextures(1, &m_BufferID);
	glBindTexture(GL_TEXTURE_2D, m_BufferID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	switch (m_Bitdepth) {
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
			break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
			break;
		case 1:
			// Grayscale images
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, m_Width, m_Height, 0, GL_RED, GL_UNSIGNED_BYTE, buffer);
			break;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}
