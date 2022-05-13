#pragma once
#include "Graphics/Sprite.h"

namespace SIN {

	struct SpriteComponent //Wrapper for Sprite, to be used in entt
	{
	public:
		SpriteComponent() : m_Sprite(nullptr) {}
		SpriteComponent(const std::string& texPath, const glm::vec2& offset = glm::vec2())
		{
			m_Sprite = new Sprite(texPath, offset);
		}
		~SpriteComponent() { /*delete m_Sprite;*/ } //Leave this empty, entt calls it at weird times.
		const Sprite* GetSprite() const { return m_Sprite; }
		void SetSprite(Sprite* s) 
		{ 
			m_Sprite = s; 
		}
	private:
		Sprite* m_Sprite;
	};

}