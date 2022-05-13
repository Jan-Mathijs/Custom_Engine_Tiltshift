#pragma once
#include <string>
#include <glm/glm/glm.hpp> //I don't know how to forward declare glm structs
#include "Graphics/Renderer.h" //This inclusion is necessary, since it defines Renderer::Vertex

namespace sce {
	namespace Gnm {
		class Buffer;
	}
}

namespace SIN {

	class Texture;
	class Sprite
	{
	public:
		Sprite();
		Sprite(const std::string& texPath, const glm::vec2& offsetFromModel = glm::vec2());
		~Sprite();
		
		void SetTexture(const std::string& a_texPath);
		const Texture* GetTexture() const { return m_Texture; }
		const Renderer::IndexSize* GetIndexBuffer() const { return m_Indices; }
		const glm::vec2& GetOffsetFromModel() const { return m_OffsetFromModel; }

	private:
		glm::vec2 m_OffsetFromModel;
		Texture* m_Texture;
		Renderer::Vertex* m_Vertices;
		Renderer::IndexSize* m_Indices;

#ifdef PLATFORM_WIN64
	public:
		unsigned int GetBufferID() const { return m_BufferID; }
	private:
		unsigned int m_BufferID;
#endif
	};
}