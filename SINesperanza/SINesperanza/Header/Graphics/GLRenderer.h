#pragma once
#include "Graphics/Renderer.h"
#include <string>

namespace SIN {

    /// <summary>
    /// Contains OpenGL implementation of Renderer
    /// </summary>
    
    class Sprite;
    class GLRenderer :
        public Renderer
    {
    public:
        GLRenderer();
        virtual ~GLRenderer();
        void Render(int a_CameraIndex);

    private:
        //Internal functions that deal with OpenGL errors
        void ClearErrors();
        void CheckErrors(const char* funcName);

        //Internal functions that deal with shader loading
        unsigned int CompileShaderProgram(const std::string& vertShaderPath, const std::string& fragShaderPath);
        std::string ParseShader(const std::string& shaderFilepath);
        unsigned int CompileSingleShader(unsigned int type, const std::string& shaderSrc);

        //OpenGL variables for rendering points
        void InitPointContext(); //Sets up the variables below
        float g_DefaultPointSize;
        unsigned int m_PointShaderPO;
        unsigned int m_PointsVAO;
        unsigned int m_PointBuffer;

        //OpenGL variables for rendering lines
        void InitLineContext(); //Sets up the variables below
        unsigned int m_LineShaderPO;
        unsigned int m_LinesVAO;
        unsigned int m_LineBuffer;

        //OpenGL variables for rendering sprites
        void InitSpriteContext(); //Sets up the variables below
        unsigned int m_SpriteShaderPO;
        unsigned int m_SpritesVAO;
    };

}