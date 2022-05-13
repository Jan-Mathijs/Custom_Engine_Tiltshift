#pragma once

#include <string>
#include <glm/glm/glm.hpp>

namespace SIN {

	struct PixelBuffer
	{
		int width, height, bitdepth;
		unsigned char* data;

		// Creates a empty pixel buffer
		PixelBuffer()
		{
			width = 0;
			height = 0;
			bitdepth = 0;
			data = 0;
		}

		// Creates a pixel buffer with the given size and bitdepth and fills it with white pixels
		PixelBuffer(int a_Width, int a_Height, int a_Bitdepth)
		{
			width = a_Width;
			height = a_Height;
			bitdepth = a_Bitdepth;

			// Create a white texture
			unsigned int size = width * height * bitdepth;
			data = new unsigned char[size];
			for (unsigned int i = 0; i < size; i++) {
				data[i] = 255;
			}
		}

		~PixelBuffer()
		{
			delete[] data;
		}

		// Sets the pixel at the location th the given color (Values from 0 to 255)
		void SetPixelColor(int a_X, int a_Y, glm::vec4 a_Color)
		{
			int pos = ((a_Y * width) + a_X) * bitdepth; // Position of pixel in buffer
			if (pos > width * height * bitdepth || pos < 0) return; // Out of buffer bounds

			if (bitdepth == 1) {
				int value = static_cast<int>(a_Color.r + a_Color.g + a_Color.b) / 3;
				data[pos] = value;
			}
			if (bitdepth == 3 || bitdepth == 4) {
				data[pos] = static_cast<unsigned char>(a_Color.r);
				data[pos + 1] = static_cast<unsigned char>(a_Color.g);
				data[pos + 2] = static_cast<unsigned char>(a_Color.b);
			}
			if (bitdepth == 4) {
				data[pos + 3] = static_cast<unsigned char>(a_Color.a);
			}
		}

		// Get the color of the pixel at the given location (Values from 0 to 255)
		glm::vec4 GetPixelColor(int a_X, int a_Y)
		{
			glm::vec4 color(255, 255, 255, 255);
			int pos = ((a_Y * width) + a_X) * bitdepth; // Position of pixel in buffer
			if (pos > width * height * bitdepth || pos < 0) return color; // Out of buffer bounds, returning default white color

			if (bitdepth == 1) {
				color = glm::vec4(data[pos]);
			}
			if (bitdepth == 3 || bitdepth == 4) {
				color.r = data[pos];
				color.g = data[pos + 1];
				color.b = data[pos + 2];
			}
			if (bitdepth == 4) {
				color.a = data[pos + 3];
			}

			return color;
		}
	};

	class Texture
	{
	public:
		virtual ~Texture() = 0;

		// Factory methods
		static Texture* CreateTexture();
		static Texture* CreateTexture(const std::string& a_FilePath, bool a_StorePixelBuffer = false);
		static Texture* CreateTexture(PixelBuffer* a_PixelBuffer);

		const int GetWidth() const { return m_Width; }
		const int GetHeight() const { return m_Height; }

		PixelBuffer* GetPixelBuffer() { return m_PixelBuffer; }

		virtual const unsigned int GetBufferID() const { return 0; }

	protected:
		Texture();

		virtual void LoadResource(const std::string& a_FilePath, bool a_StorePixelBuffer) = 0;
		virtual void LoadFromPixelBuffer(PixelBuffer* a_PixelBuffer) = 0;

		int m_Width, m_Height, m_Bitdepth;
		PixelBuffer* m_PixelBuffer;
	};
}