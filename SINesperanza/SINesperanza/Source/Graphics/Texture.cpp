#include "Graphics/Texture.h"
#ifdef PLATFORM_WIN64
#include "Graphics/GLTexture.h"
#endif

using namespace SIN;

Texture::Texture() :
	m_Width(0), m_Height(0), m_Bitdepth(4), m_PixelBuffer(nullptr)
{

}

Texture::~Texture()
{
	if (m_PixelBuffer) {
		delete m_PixelBuffer;
		m_PixelBuffer = nullptr;
	}
}

// Factory Methods
Texture* Texture::CreateTexture()
{
#ifdef PLATFORM_WIN64
	Texture* texture = new GLTexture();
#endif
	return texture;
}

Texture* Texture::CreateTexture(const std::string& a_FilePath, bool a_StorePixelBuffer)
{
#ifdef PLATFORM_WIN64
	Texture* texture = new GLTexture();
#endif
	texture->LoadResource(a_FilePath, a_StorePixelBuffer);

	return texture;
}

Texture* Texture::CreateTexture(PixelBuffer* a_PixelBuffer)
{
#ifdef PLATFORM_WIN64
	Texture* texture = new GLTexture();
#endif
	texture->LoadFromPixelBuffer(a_PixelBuffer);

	return texture;
}
