#pragma once

#include <stb/stb_image.h>
#include "shader.hpp"
#include "ft2build.h"
#include FT_FREETYPE_H  


static unsigned int windowHeight = 1080;
static unsigned int windowWidth = 1920;


namespace Renderer {


	void Init2D();
	void Shutdown();
	void BeginBatch();
	void EndBatch();
	void Flush();
	GLuint LoadTexture(const char* imageFilePath);
	void DrawQuad(const glm::vec2& position, const glm::vec2 size, const glm::vec4& color, GLuint textureID);

	static const uint32_t MAX_QUAD_COUNT = 1000;
	static const uint32_t MAX_VERTEX_COUNT = MAX_QUAD_COUNT * 4;
	static const uint32_t MAX_INDEX_COUNT = MAX_QUAD_COUNT * 6;
	static const uint8_t MAX_TEXTURE_COUNT = 32;


	struct Vertex {
		glm::vec2 Position;
		glm::vec4 Color;
		glm::vec2 TexCoords;
		float TexIndex;
		float IsText;
	};

	struct RendererData {
		bool Initilized = false;

		Shader QuadShader{};
		GLuint VAO = 0;
		GLuint VBO = 0;
		GLuint IBO = 0;

		uint32_t IndexCount = 0;

		Vertex* Buffer = nullptr;
		Vertex* BufferPtr = nullptr;

		GLuint WhiteTextureID = 0;
		uint32_t WhiteTextureSlot = 0;

		std::array<uint32_t, MAX_TEXTURE_COUNT> TextureSlots{ 0 };
		uint32_t TextureSlotIndex = 1;
	};
	extern RendererData Data;

	class Text {
	public:
		void Init(const char* fontFilePath, int height);
		enum class HorizontalAlignment {
			Left,
			Center,
			Right,
		};
		enum class VerticalAlignment {
			Top,
			Center,
			Bottom,
		};
		void Write(const std::string& text, glm::vec2 position, glm::vec2 size, const glm::vec4& color, HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left, VerticalAlignment verticalAlignment = VerticalAlignment::Top);
	private:
		static const int MAXWIDTH = 4096;
		float TextureIndex_;

		unsigned int TextureWidth_ = 0;
		unsigned int TextureHeight_ = 0;

		struct {
			glm::vec2 advance;

			float bitmapW;
			float bitmapH;

			float bitmapL;
			float bitmapT;
			glm::vec2 textureOffset;
		} Characters_[128];

		float Height_ = 0.0f;
	};

};


