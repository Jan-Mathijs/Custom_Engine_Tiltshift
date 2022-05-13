#include "Graphics/Sprite.h"
#include "Graphics/Texture.h"

#ifdef PLATFORM_WIN64
#include <GLEW/glew.h>
#endif

using namespace SIN;

Sprite::Sprite() :
	m_OffsetFromModel(glm::vec2()), m_Texture(nullptr), m_Vertices(nullptr), m_Indices(nullptr)
{
#ifdef PLATFORM_WIN64
	m_BufferID = 0;
#endif
}

Sprite::Sprite(const std::string& texPath, const glm::vec2& offsetFromModel) :
	m_OffsetFromModel(offsetFromModel)
{
#ifdef PLATFORM_WIN64
	m_Vertices = (Renderer::Vertex*)malloc(sizeof(Renderer::Vertex) * 4);
	m_Indices = (Renderer::IndexSize*)malloc(sizeof(Renderer::IndexSize) * 6);
#endif

	m_Texture = Texture::CreateTexture(texPath);

	int rectW = m_Texture->GetWidth(), rectH = m_Texture->GetHeight();
	if (rectW && rectH)
	{
		float divisor = static_cast<float>(std::max(rectW, rectH)) * 2.f;
		m_Vertices[0] = Renderer::Vertex(-rectW / divisor, -rectH / divisor, 0.f, 0.f);
		m_Vertices[1] = Renderer::Vertex(rectW / divisor, -rectH / divisor, 1.f, 0.f);
		m_Vertices[2] = Renderer::Vertex(rectW / divisor, rectH / divisor, 1.f, 1.f);
		m_Vertices[3] = Renderer::Vertex(-rectW / divisor, rectH / divisor, 0.f, 1.f);
	}

	m_Indices[0] = static_cast<uint16_t>(0);
	m_Indices[1] = static_cast<uint16_t>(1);
	m_Indices[2] = static_cast<uint16_t>(2);
	m_Indices[3] = static_cast<uint16_t>(2);
	m_Indices[4] = static_cast<uint16_t>(3);
	m_Indices[5] = static_cast<uint16_t>(0);

#ifdef PLATFORM_WIN64
	glGenBuffers(1, &m_BufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Renderer::Vertex) * 4, m_Vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
}

Sprite::~Sprite()
{
	if (m_Texture)
	{
		delete m_Texture;
	}

#ifdef PLATFORM_WIN64
	glDeleteBuffers(1, &m_BufferID);
	free(m_Vertices);
	free(m_Indices);
#endif
}

void Sprite::SetTexture(const std::string& a_texPath)
{
	if (m_Texture)
		delete m_Texture;
	m_Texture = Texture::CreateTexture(a_texPath);
}