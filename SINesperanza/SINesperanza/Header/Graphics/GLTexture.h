#pragma once
#include <string>
#include "Texture.h"

namespace SIN {
	class GLTexture : public Texture {
	public:
		GLTexture() : m_BufferID(0)
		{}
		virtual ~GLTexture();

		void LoadResource(const std::string& a_FilePath, bool a_StorePixelBuffer);
		void LoadFromPixelBuffer(PixelBuffer* a_PixelBuffer);
		const unsigned int GetBufferID() const override { return m_BufferID; }

	private:
		void GenOpenGLTexture(unsigned char* buffer);

		unsigned int m_BufferID;
	};
}