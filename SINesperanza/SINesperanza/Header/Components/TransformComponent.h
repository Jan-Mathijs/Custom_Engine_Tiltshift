#pragma once
#include "glm/glm/glm.hpp"

namespace SIN
{
	struct TransformComponent
	{
	public:
		const glm::vec2& GetWorldLocation() const { return m_WorldLocation; }
		const glm::vec2& GetDirection() const { return m_Direction; }
		const glm::vec2& GetRightDirection() const { return m_RightDirection; }
		float            GetZRotation() const { return m_ZRotation; }
		const glm::vec2& GetScale() const { return m_Scale; }

		void SetWorldLocation(glm::vec2 a_worldLocation) { m_WorldLocation = a_worldLocation; }
		void SetDirection(glm::vec2 a_Direction) { m_Direction = a_Direction; }
		void SetRightDirection(glm::vec2 a_rightDirection) { m_RightDirection = a_rightDirection; }
		//Specify in degrees; rotates clockwise
		void SetZRotation(float a_rotation) { m_ZRotation = a_rotation; }
		void SetScale(glm::vec2 a_scale) { m_Scale = a_scale; }

		glm::mat4 GetTransformMatrix() const
		{
			glm::mat4 translation(1.f);
			translation[3][0] = m_WorldLocation.x;
			translation[3][1] = m_WorldLocation.y;

			glm::mat4 rotation(1.f);
			rotation[0][0] = glm::cos(glm::radians(m_ZRotation));
			rotation[0][1] = glm::sin(glm::radians(m_ZRotation));
			rotation[1][0] = -glm::sin(glm::radians(m_ZRotation));
			rotation[1][1] = glm::cos(glm::radians(m_ZRotation));

			glm::mat4 scale(1.f);
			scale[0][0] *= m_Scale.x;
			scale[1][1] *= m_Scale.y;

			return translation * rotation * scale;
		}

	private:
		glm::vec2 m_WorldLocation = glm::vec2(0);
		glm::vec2 m_Direction = glm::vec2(0, 1);
		glm::vec2 m_RightDirection = glm::vec2(1, 0);
		float     m_ZRotation = 0;
		glm::vec2 m_Scale = glm::vec2(1);
	};
}
