#pragma once
#include "glm/glm/glm.hpp"

struct AINode
{
public:

	glm::vec2 m_Direction = glm::vec2(0.f, 1.0f);
	float m_Speed = 0.1f;
	glm::vec2 m_Position = glm::vec2(1);
	float m_PosOnLine = -1;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(m_Direction.x, m_Direction.y, m_Position.x, m_Position.y, m_Speed); // serialize things by passing them to the archive
	}
	
	AINode(glm::vec2 a_Pos) { m_Position = a_Pos; }
	AINode(glm::vec2 a_Pos, glm::vec2 a_Dir, float t) { m_Position = a_Pos; m_Direction = a_Dir; m_PosOnLine = t; }
	AINode(glm::vec2 a_Pos, glm::vec2 a_Dir, float a_Speed, float t) { m_Position = a_Pos; m_Direction = a_Dir; m_Speed = a_Speed; m_PosOnLine = t; }
	AINode(){}
	~AINode(){}
};