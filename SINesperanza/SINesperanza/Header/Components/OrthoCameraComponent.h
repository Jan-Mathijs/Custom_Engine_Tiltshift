#pragma once
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/quaternion.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

namespace SIN {

	class OrthoCameraComponent {
	public:
		OrthoCameraComponent(float a_ExtentX, float a_ExtentY) :
			m_ExtentX(a_ExtentX), m_ExtentY(a_ExtentY)
		{
			m_ProjMatrix = glm::ortho(-m_ExtentX / 2.f, m_ExtentX / 2.f, -m_ExtentY / 2.f, m_ExtentY / 2.f, -1.f, 1.f);
		}

		~OrthoCameraComponent() = default;

		//Change these if you want to zoom in or out
		void SetViewBounds(float a_NewExtentX, float a_NewExtentY) 
		{ 
			m_ExtentX = a_NewExtentX;
			m_ExtentY = a_NewExtentY;
			m_ProjMatrix = glm::ortho(-m_ExtentX / 2.f, m_ExtentX / 2.f, -m_ExtentY / 2.f, m_ExtentY / 2.f, -1.f, 1.f);
		}

		const glm::mat4& GetProjectionMatrix() const { return m_ProjMatrix; }
		
		//CameraRot in degrees
		const glm::mat4 GetViewMatrix(glm::vec2 a_CameraPos, float a_CameraRot) const
		{
			glm::vec3 up(-glm::sin(glm::radians(a_CameraRot)), glm::cos(glm::radians(a_CameraRot)), 0.f);
			//z position of the camera is always 1.f. Change view extent if you want to zoom in or out.
			return glm::lookAt(glm::vec3(a_CameraPos, 1.f), glm::vec3(a_CameraPos, 0.f), up);
		}

	private:
		float m_ExtentX, m_ExtentY;
		glm::mat4 m_ProjMatrix;
	};

}