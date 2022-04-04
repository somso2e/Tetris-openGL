#pragma once
#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <common.hpp>
#include <stb/stb_image.h>
#include "shader.hpp"

static const int MAX_QUAD_COUNT = 1000;
static const int MAX_VERTEX_COUNT = MAX_QUAD_COUNT * 4;
static const int MAX_INDEX_COUNT = MAX_QUAD_COUNT * 6;
static const int MAX_TEXTURE_COUNT = 32;

namespace Renderer {


	void Init();
	void Shutdown();
	void BeginBatch();
	void EndBatch();
	void Flush();
	GLuint LoadTexture(const char* imageFilePath);
	void DrawQuad(const glm::vec2& position, const glm::vec2 size, const glm::vec4& color, uint32_t textureID);

	struct RendererData {
		Shader Shader;
		GLuint VAO = 0;
		GLuint VBO = 0;
		GLuint IBO = 0;

		uint32_t IndexCount = 0;

		Vertex* Buffer = nullptr;
		Vertex* BufferPtr = nullptr;

		GLuint WhiteTextureID = 0;
		uint32_t WhiteTextureSlot = 0;

		std::array<uint32_t, MAX_TEXTURE_COUNT> TextureSlots;
		uint32_t TextureSlotIndex = 1;
	};
	static RendererData s_Data;


};

#endif