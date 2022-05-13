#pragma once
#include <glm/glm/vec2.hpp>
#include "Components/TireComponent.h"

namespace SIN
{
	enum class AIState
	{
		Accelerate,
		Brake,
		Natural
	};

	struct AIComponent
	{
	public:
		float GetMass() { return m_Mass; }
		float GetCurrentSpeed() { return m_CurrentSpeed; }
		float GetMaxSpeed() { return m_MaxSpeed; }
		float GetAcceleration() { return m_Acceleration; }
		float GetBreakingForce() { return m_BreakingForce; }
		AIState GetAIState() { return m_AIState; }
		float GetPositionOnSpline() { return m_positionOnSpline; }
		int GetCurrentNode() { return m_CurrentNode; }
		int GetNodeReadSpeed() { return m_NodeReadSpeed; }
		
		void SetMass(float a_mass) { m_Mass = a_mass; }
		void SetCurrentSpeed(float a_speed) { m_CurrentSpeed = a_speed; }
		void SetBreakingForce(float a_BreakingFroce) { m_BreakingForce = a_BreakingFroce; }
		void SetAIState(AIState a_state) { m_AIState = a_state; }
		void SetPositionOnSpline(float a_position) { m_positionOnSpline = a_position; }
		void SetAcceleration(float a_acceleration) { m_Acceleration = a_acceleration; }
		void SetCurrentNode(int a_index) { m_CurrentNode = a_index; }
		

		glm::vec2 m_ForwardSpeed = glm::vec2(0);
		glm::vec2 m_LateralSpeed = glm::vec2(0);

		int GetAIIndex() { return m_Count; }
		void SetAIIndex(int a_Count) { m_Count = a_Count; }

		void SetRestart(bool restart) { m_Restart = restart; }

		bool GetRestart() { return m_Restart; }

		b2RevoluteJoint* m_FlJoint;
		b2RevoluteJoint* m_FrJoint;

	private:
		float m_Mass = 500.0f;

		float m_CurrentSpeed = 0.f;
		float m_MaxSpeed = 200.0f;
		float m_Acceleration = 6.0f;
		float m_BreakingForce = 2000.0f;
		float m_BreakingStartSpeed = 0.0f;
		AIState m_AIState = AIState::Natural;
		float m_positionOnSpline = 0.0f;
		int m_Count = -1;

		int m_NodeReadSpeed = 1;
		int m_CurrentNode = 0;
		int m_restartCounter = 0;
		bool m_Restart = false;
	};
}
