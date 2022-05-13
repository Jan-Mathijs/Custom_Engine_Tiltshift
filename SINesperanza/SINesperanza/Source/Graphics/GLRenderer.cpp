#pragma warning(push)
#pragma warning(disable : 4103 26495 26451)

#include "Graphics/GLRenderer.h"

#define GLEW_STATIC
#include <GLEW/glew.h>

#include <glm/glm/matrix.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Graphics/Texture.h"
#include "Graphics/Sprite.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"
#include "Components/OrthoCameraComponent.h"
#include <fstream>

#pragma warning(pop)

using namespace SIN;

GLRenderer::GLRenderer() :
	Renderer(),
	g_DefaultPointSize(1.f),
	m_PointShaderPO(0),
	m_PointsVAO(0),
	m_PointBuffer(0),
	m_LineShaderPO(0),
	m_LinesVAO(0),
	m_LineBuffer(0),
	m_SpriteShaderPO(0),
	m_SpritesVAO(0)
{
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		printf("Couldn't initialise GLEW!!1!");
		return;
	}

	//Set up some GL context values

	InitPointContext();
	InitLineContext();
	InitSpriteContext();

	//Test code
	/*auto entity = CreateEntity();
	GetEntityRegister().emplace<SpriteComponent>(entity, "Assets/Textures/BlueberryHead.png", glm::vec2(-100.f, 0.f));
	TransformComponent& _transform = GetEntityRegister().emplace<TransformComponent>(entity);
	_transform.SetWorldLocation(glm::vec2(10.f, 10.f));
	_transform.SetScale(glm::vec2(10.f, 10.f));
	auto entity2 = CreateEntity();
	GetEntityRegister().emplace<SpriteComponent>(entity2, "Assets/Textures/BlueberryHead.png");
	TransformComponent& _transform2 = GetEntityRegister().emplace<TransformComponent>(entity2);
	_transform2.SetWorldLocation(glm::vec2(-1.f, -1.f));
	_transform2.SetScale(glm::vec2(10.f, 10.f));*/
}

GLRenderer::~GLRenderer()
{

}

void GLRenderer::Render(int a_CameraIndex)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glm::mat4 vp;
	auto cameras = GetEntityRegister().view<TransformComponent, OrthoCameraComponent>();
	int counter = static_cast<int>(cameras.size()) - 1;
	if (counter < 0) //There's no camera
	{
		return;
	}
	for (auto camera : cameras) //entt gives you the list of cameras in reverse order
	{
		if (counter == a_CameraIndex)
		{
			TransformComponent cameraTransform = GetEntityRegister().get<TransformComponent>(camera);
			OrthoCameraComponent cameraComp = GetEntityRegister().get<OrthoCameraComponent>(camera);
			vp = cameraComp.GetProjectionMatrix() * cameraComp.GetViewMatrix(cameraTransform.GetWorldLocation(), cameraTransform.GetZRotation());
		}
		counter--;
	}

	if (m_NumPoints > 0)
	{ //Render points
		glEnable(GL_PROGRAM_POINT_SIZE);
		glBindVertexArray(m_PointsVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_PointBuffer);
		//This is slow, but it's a debug renderer, so w/e
		glBufferData(GL_ARRAY_BUFFER, sizeof(Renderer::Point) * m_NumPoints, m_Points, GL_DYNAMIC_DRAW);
		glUseProgram(m_PointShaderPO);
		glUniformMatrix4fv(glGetUniformLocation(m_LineShaderPO, "u_VP"), 1, GL_FALSE, glm::value_ptr(vp));
		glDrawArrays(GL_POINTS, 0, m_NumPoints);

		m_NumPoints = 0; //Reset the Points buffer
		glDisable(GL_PROGRAM_POINT_SIZE);
	}

	if (m_NumLines > 0)
	{ //Render lines
		glBindVertexArray(m_LinesVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_LineBuffer);
		//This is slow, but it's a debug renderer, so w/e
		glBufferData(GL_ARRAY_BUFFER, sizeof(Renderer::Line) * m_NumLines, m_Lines, GL_DYNAMIC_DRAW);
		glUseProgram(m_LineShaderPO);
		glUniformMatrix4fv(glGetUniformLocation(m_LineShaderPO, "u_VP"), 1, GL_FALSE, glm::value_ptr(vp));
		glDrawArrays(GL_LINES, 0, m_NumLines * 2);

		m_NumLines = 0; //Reset the Lines buffer
	}
	
	{//Render sprites
		auto view = GetEntityRegister().view<SpriteComponent>();
		for (entt::entity entity : view)
		{
			const Sprite* sprite = GetEntityRegister().get<SpriteComponent>(entity).GetSprite();

			if (sprite)
			{
				glBindVertexArray(m_SpritesVAO);

				const TransformComponent& transf = GetEntityRegister().get<TransformComponent>(entity);
				glm::mat4 transformMatrix = transf.GetTransformMatrix();
				glm::vec2 offset = sprite->GetOffsetFromModel();
				if (offset != glm::vec2())
				{
					transformMatrix[3].x += offset.x * glm::cos(glm::radians(transf.GetZRotation())) - offset.y * glm::sin(glm::radians(transf.GetZRotation()));
					transformMatrix[3].y += offset.y * glm::cos(glm::radians(transf.GetZRotation())) - offset.x * glm::sin(glm::radians(transf.GetZRotation()));
				}
				glm::mat4 mvp = vp * transformMatrix;
				glBindBuffer(GL_ARRAY_BUFFER, sprite->GetBufferID());

				//Set up how the texture data should be read; this needs to be done each time a different buffer is bound
				GLint loc = glGetAttribLocation(m_SpriteShaderPO, "a_Position");
				glEnableVertexAttribArray(loc);
				glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(Renderer::Vertex),
					(const void*)offsetof(Renderer::Vertex, Renderer::Vertex::X));
				loc = glGetAttribLocation(m_SpriteShaderPO, "a_TexCoord");
				glEnableVertexAttribArray(loc);
				glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(Renderer::Vertex),
					(const void*)offsetof(Renderer::Vertex, Renderer::Vertex::TexX));

				glBindTexture(GL_TEXTURE_2D, sprite->GetTexture()->GetBufferID());

				glUseProgram(m_SpriteShaderPO);
				glUniformMatrix4fv(glGetUniformLocation(m_SpriteShaderPO, "u_MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
				glUniform1i(glGetUniformLocation(m_SpriteShaderPO, "u_TextureSlot"), 0);
				//glDrawArrays(GL_TRIANGLES, 0, 6);
				
				GLenum indexSize;
				switch (sizeof(Renderer::IndexSize))
				{
				case 4:
					indexSize = GL_UNSIGNED_INT;
					break;
				case 2:
				default:
					indexSize = GL_UNSIGNED_SHORT;
					break;
				}
				glDrawElements(GL_TRIANGLES, 6, indexSize, sprite->GetIndexBuffer());
			}
		}
	}
	
	//clean up
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

unsigned int GLRenderer::CompileShaderProgram(const std::string& vertShaderPath, const std::string& fragShaderPath)
{
	unsigned int program = glCreateProgram();
	std::string vertShader = ParseShader(vertShaderPath);
	std::string fragShader = ParseShader(fragShaderPath);
	//std::cout << "VERTEX:" << std::endl << vertShader << std::endl; //Debug line
	//std::cout << "FRAGMENT:" << std::endl << fragShader << std::endl << std::endl; //Debug line
	unsigned int vs = CompileSingleShader(GL_VERTEX_SHADER, vertShader);
	unsigned int fs = CompileSingleShader(GL_FRAGMENT_SHADER, fragShader);
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	int programLog = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &programLog);
	if (programLog != GL_TRUE)
	{
		printf("Whoa, this program cannot link correctly!\n");
		char logInfo[256];
		int length;
		glGetProgramInfoLog(program, 256, &length, logInfo);
		printf("Program linking info: %s\n", logInfo);

		glDeleteShader(vs);
		glDeleteShader(fs);
		glDeleteProgram(program);
		return 0;
	}

	glValidateProgram(program);
	programLog = 0;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &programLog);
	if (programLog != GL_TRUE)
	{
		printf("Whoa, this program is invalid!\n");
		char logInfo[256];
		int length;
		glGetProgramInfoLog(program, 256, &length, logInfo);
		printf("Program validation info: %s\n", logInfo);

		glDeleteShader(vs);
		glDeleteShader(fs);
		glDeleteProgram(program);
		return 0;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

std::string GLRenderer::ParseShader(const std::string& filepath)
{
	std::ifstream filestream(filepath);
	std::string line;
	std::stringstream shaderCode;

	while (getline(filestream, line))
	{
		shaderCode << line << std::endl;
	}

	return shaderCode.str();
}

unsigned int GLRenderer::CompileSingleShader(unsigned int type, const std::string& shaderSrc)
{
	unsigned int shaderID = glCreateShader(type);
	const char* src = shaderSrc.c_str();
	glShaderSource(shaderID, 1, &src, nullptr);
	glCompileShader(shaderID);
	int valid;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &valid);
	if (valid != GL_TRUE)
	{
		printf("Whoa, this %s shader is invalid!\n", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"));
		char logInfo[256];
		int length;
		glGetShaderInfoLog(shaderID, 256, &length, logInfo);
		printf("Shader info: %s\n", logInfo);
		glDeleteShader(shaderID);
		return 0;
	}
	return shaderID;
}

void GLRenderer::ClearErrors()
{
	while (glGetError());
}

void GLRenderer::CheckErrors(const char* funcName)
{
	const char* errorCode;
	GLenum error;
#pragma warning(push)
#pragma warning(disable : 4706) //MSVC complaining about intended behaviour
	while (error = glGetError())
	{
		switch (error)
		{
		case 1280: errorCode = "GL_INVALID_ENUM\0"; break;
		case 1281: errorCode = "GL_INVALID_VALUE\0"; break;
		case 1282: errorCode = "GL_INVALID_OPERATION\0"; break;
		case 1283: errorCode = "GL_STACK_OVERFLOW\0"; break;
		case 1284: errorCode = "GL_STACK_UNDERFLOW\0"; break;
		case 1285: errorCode = "GL_OUT_OF_MEMORY\0"; break;
		default: errorCode = "GL_UNKNOWN_ERROR\0"; break;
		}
		printf("GL error in %s: %s\n", funcName, errorCode);
	}
#pragma warning(pop)
}

void GLRenderer::InitPointContext()
{
	//Store the default point size (should usually be 1.f)
	glGetFloatv(GL_POINT_SIZE, &g_DefaultPointSize);

	//Set up a shader program for points (FragShaderLine makes everything white, which is fine for points too)
	m_PointShaderPO = CompileShaderProgram("Assets/Shaders/VertShaderPoint.glsl", "Assets/Shaders/FragShaderLine.glsl");

	//Make a VAO for the point buffer
	glCreateVertexArrays(1, &m_PointsVAO);
	glBindVertexArray(m_PointsVAO);

	//Make point buffer and set its size
	glGenBuffers(1, &m_PointBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_PointBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Renderer::Point) * Renderer::g_MaxPointsLines, nullptr, GL_DYNAMIC_DRAW);

	//Set up how the point data is to be read
	GLint loc = glGetAttribLocation(m_PointShaderPO, "a_Position");
	if (loc < 0) //Debug line
	{
		printf("Whoa, shader attribute location not found!\n");
	}
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(Renderer::Point),
		(const void*)offsetof(Renderer::Point, Renderer::Point::X)); //offsetof finds the offset of X in Point (in bytes)

	loc = glGetAttribLocation(m_PointShaderPO, "a_PointSize");
	if (loc < 0) //Debug line
	{
		printf("Whoa, shader attribute location not found!\n");
	}
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 1, GL_FLOAT, GL_FALSE, sizeof(Renderer::Point),
		(const void*)offsetof(Renderer::Point, Renderer::Point::Size));

	//Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GLRenderer::InitLineContext()
{
	//Set up a shader program for lines
	m_LineShaderPO = CompileShaderProgram("Assets/Shaders/VertShaderLine.glsl", "Assets/Shaders/FragShaderLine.glsl");

	//Make a VAO for the line buffer
	glCreateVertexArrays(1, &m_LinesVAO);
	glBindVertexArray(m_LinesVAO);

	//Make the line buffer and set its size
	glGenBuffers(1, &m_LineBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_LineBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Renderer::Line) * Renderer::g_MaxPointsLines, nullptr, GL_DYNAMIC_DRAW);
	
	//Set up how the line data is to be read
	GLint loc = glGetAttribLocation(m_PointShaderPO, "a_Position");
	if (loc < 0) //Debug line
	{
		printf("Whoa, shader attribute location not found!\n");
	}
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(Renderer::Line) / 2,
		(const void*)offsetof(Renderer::Line, Renderer::Line::X0));

	//Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GLRenderer::InitSpriteContext()
{
	//Set up a shader program for sprites
	m_SpriteShaderPO = CompileShaderProgram("Assets/Shaders/VertShaderTex.glsl", "Assets/Shaders/FragShaderTex.glsl");

	//Make a VAO for texture buffers
	glCreateVertexArrays(1, &m_SpritesVAO);
}