#pragma once

#include "common.hpp"
#include <stb/stb_image.h>
#include "shader.hpp"
#include "ft2build.h"
#include FT_FREETYPE_H  

static const uint32_t MAX_QUAD_COUNT = 1000;
static const uint32_t MAX_VERTEX_COUNT = MAX_QUAD_COUNT * 4;
static const uint32_t MAX_INDEX_COUNT = MAX_QUAD_COUNT * 6;
static const uint8_t MAX_TEXTURE_COUNT = 32;

namespace Renderer {


	void Init2D();
	void Shutdown();
	void BeginBatch();
	void EndBatch();
	void Flush();
	GLuint LoadTexture(const char* imageFilePath);
	void DrawQuad(const glm::vec2& position, const glm::vec2 size, const glm::vec4& color, uint32_t textureID);

	struct Vertex {
		glm::vec2 Position;
		glm::vec4 Color;
		glm::vec2 TexCoords;
		float TexIndex;
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
	static RendererData s_Data;


	typedef FT_Face Font;
	Font InitText(const char* fontFileName);

	class Atlas {
	public:
		void Create(Font font, int height);
		void Draw(std::string text, glm::vec2 position, glm::vec2 size, const glm::vec4& color);
	private:
		//FT_Face Face;
		static const int MAXWIDTH = 1024;
		GLuint TextureID;		// texture object

		unsigned int textureWidth = 0;			// width of texture in pixels
		unsigned int textureHeight = 0;			// height of texture in pixels

		struct {
			glm::vec2 advance;

			float bitmapW;
			float bitmapH;

			float bitmapL;
			float bitmapT;
			glm::vec2 textureOffset;
		} Characters[128];

	};

};


