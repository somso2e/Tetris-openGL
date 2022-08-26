#include "renderer.hpp"

Renderer::RendererData Renderer::Data;


void Renderer::Init2D() {
	// To avoid accidental reinitialization
	if (Data.Initilized)
		return;
	Data.Initilized = true;

	// Initilize the shader
	Data.QuadShader.Compile("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
	Data.QuadShader.Use();
	GLint samplers[MAX_TEXTURE_COUNT];
	for (int i = 0; i < MAX_TEXTURE_COUNT; i++) {
		samplers[i] = i;
	}
	Data.QuadShader.SetUniform1iv("uTextures", MAX_TEXTURE_COUNT, samplers);

	// Set vertex attributes
	Data.Buffer = new Vertex[MAX_VERTEX_COUNT];

	glCreateVertexArrays(1, &Data.VAO);
	glBindVertexArray(Data.VAO);

	glCreateBuffers(1, &Data.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, Data.VBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_COUNT * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexArrayAttrib(Data.VAO, 0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Vertex::Position));

	glEnableVertexArrayAttrib(Data.VAO, 1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Vertex::Color));

	glEnableVertexArrayAttrib(Data.VAO, 2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Vertex::TexCoords));

	glEnableVertexArrayAttrib(Data.VAO, 3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Vertex::TexIndex));

	glEnableVertexArrayAttrib(Data.VAO, 4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Vertex::IsText));

	// Set Indices 
	uint32_t indices[MAX_INDEX_COUNT]{ 0 };
	uint32_t offset = 0;
	for (int i = 0; i < MAX_INDEX_COUNT; i += 6) {
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;

		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;
		offset += 4;
	}
	glCreateBuffers(1, &Data.IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Data.IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Set default white texture 
	glCreateTextures(GL_TEXTURE_2D, 1, &Data.WhiteTextureID);
	glBindTexture(GL_TEXTURE_2D, Data.WhiteTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	uint32_t whiteColor = 0xFFFFFFFF;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &whiteColor);

	Data.TextureSlots[0] = Data.WhiteTextureID;
}


void Renderer::Shutdown() {
	glDeleteVertexArrays(1, &Data.VAO);
	glDeleteBuffers(1, &Data.VBO);
	glDeleteBuffers(1, &Data.IBO);

	for (auto i = 0; i < MAX_TEXTURE_COUNT; i++)
		glDeleteTextures(1, &Data.TextureSlots[i]);

	delete[] Data.Buffer;
	Data.QuadShader.Delete();
}


void Renderer::BeginBatch() {
	glClearColor(0.0546f, 0.0546f, 0.0546f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	Data.QuadShader.Use();
	Data.BufferPtr = Data.Buffer;
}


void Renderer::EndBatch() {
	const glm::mat4 projection = glm::ortho(
		0.0f, static_cast<float> (windowWidth),
		static_cast<float> (windowHeight), 0.0f,
		0.0f, 1.0f);
	Data.QuadShader.SetMatrix4("projection", projection);

	GLsizeiptr size = (uint8_t*)Data.BufferPtr - (uint8_t*)Data.Buffer;
	glBindBuffer(GL_ARRAY_BUFFER, Data.VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, Data.Buffer);
}


void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2 size, const glm::vec4& color, GLuint textureID) {
	if (Data.IndexCount >= MAX_INDEX_COUNT || Data.TextureSlotIndex > 31) {
		EndBatch();
		Flush();
		BeginBatch();
	}
	float textureIndex = 0.0f;
	if (textureID != Data.WhiteTextureID) {
		for (auto i = 0; i < Data.TextureSlotIndex; i++) {
			if (Data.TextureSlots[i] == textureID) {
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f) {
			textureIndex = (float)Data.TextureSlotIndex;
			Data.TextureSlots[Data.TextureSlotIndex] = textureID;
			Data.TextureSlotIndex++;
		}
	}

	Data.BufferPtr->Position = { position.x, position.y };
	Data.BufferPtr->Color = color;
	Data.BufferPtr->TexCoords = { 0.0f,0.0f };
	Data.BufferPtr->TexIndex = textureIndex;
	Data.BufferPtr->IsText = 0.0f;
	Data.BufferPtr++;

	Data.BufferPtr->Position = { position.x + size.x, position.y };
	Data.BufferPtr->Color = color;
	Data.BufferPtr->TexCoords = { 1.0f,0.0f };
	Data.BufferPtr->TexIndex = textureIndex;
	Data.BufferPtr->IsText = 0.0f;
	Data.BufferPtr++;

	Data.BufferPtr->Position = { position.x + size.x, position.y + size.y };
	Data.BufferPtr->Color = color;
	Data.BufferPtr->TexCoords = { 1.0f,1.0f };
	Data.BufferPtr->TexIndex = textureIndex;
	Data.BufferPtr->IsText = 0.0f;
	Data.BufferPtr++;

	Data.BufferPtr->Position = { position.x, position.y + size.y };
	Data.BufferPtr->Color = color;
	Data.BufferPtr->TexCoords = { 0.0f,1.0f };
	Data.BufferPtr->TexIndex = textureIndex;
	Data.BufferPtr->IsText = 0.0f;
	Data.BufferPtr++;

	Data.IndexCount += 6;
}


GLuint Renderer::LoadTexture(const char* imageFilePath) {

	GLuint ID;
	int widthImg, heightImg, numColCh;

	unsigned char* bytes = stbi_load(imageFilePath, &widthImg, &heightImg, &numColCh, 0);
	if (!bytes) {
		log("[ERROR](Texture) Image File " + std::string(imageFilePath) + " could not be loaded.");
	}
	glCreateTextures(GL_TEXTURE_2D, 1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (numColCh == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	}
	else if (numColCh == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	}
	else {
		log("[ERROR](Texture) Invalid image type");
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	return ID;
}

void Renderer::Flush() {
	for (uint32_t i = 0; i < Data.TextureSlotIndex; i++) {
		glBindTextureUnit(i, Data.TextureSlots[i]);
	}
	glBindVertexArray(Data.VAO);
	glDrawElements(GL_TRIANGLES, Data.IndexCount, GL_UNSIGNED_INT, nullptr);
	Data.IndexCount = 0;
}


void Renderer::Text::Init(const char* fontFilePath, int height) {
	if (!Data.Initilized) {
		Renderer::Init2D();
	}
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		log("[ERROR](Text) Could not init FreeType Library");
	}
	FT_Face face;
	if (FT_New_Face(ft, fontFilePath, 0, &face)) {
		log("[ERROR](Text) Failed to load font");
	}


	FT_Set_Pixel_Sizes(face, 0, height);
	FT_GlyphSlot g = face->glyph;

	unsigned int roww = 0;
	unsigned int rowh = 0;

	unsigned int rowWidth = 0;
	unsigned int rowHeight = 0;

	memset(Characters_, 0, sizeof Characters_);

	for (int i = 32; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			log("[WARNING](Text)Loading character" + std::string(1, i) + " failed");
			continue;
		}
		rowWidth += g->bitmap.width + 1;
		rowHeight = std::max(rowHeight, g->bitmap.rows);

		if (rowWidth + g->bitmap.width + 1 >= MAXWIDTH) {
			TextureWidth_ = std::max(TextureWidth_, rowWidth);
			TextureHeight_ += rowWidth;
			rowWidth = 0;
			rowHeight = 0;
		}
	}

	TextureWidth_ = std::max(TextureWidth_, rowWidth);
	TextureHeight_ += rowHeight;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLuint textureID;
	glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, TextureWidth_, TextureHeight_, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	int offsetX = 0;
	int offsetY = 0;

	rowh = 0;

	for (int i = 32; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			//log("Loading character" << i << "failed!")
			continue;
		}
		if (offsetX + g->bitmap.width + 1 >= MAXWIDTH) {
			offsetY += rowh;
			rowh = 0;
			offsetX = 0;
		}
		glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
		Characters_[i].advance.x = (float)(g->advance.x >> 6);
		Characters_[i].advance.y = (float)(g->advance.y >> 6);

		Characters_[i].bitmapW = (float)g->bitmap.width;
		Characters_[i].bitmapH = (float)g->bitmap.rows;

		Characters_[i].bitmapL = (float)g->bitmap_left;
		Characters_[i].bitmapT = (float)g->bitmap_top;

		Characters_[i].textureOffset.x = offsetX / (float)TextureWidth_;
		Characters_[i].textureOffset.y = offsetY / (float)TextureHeight_;

		const auto& x = Characters_[i];
		rowh = std::max(rowh, g->bitmap.rows);
		offsetX += g->bitmap.width + 1;

		// get the maximum height of all characters
		Height_ = std::max(Height_, (float)g->bitmap.rows);
	}

	Characters_[' '].advance.x /= 2;

	glGenerateMipmap(GL_TEXTURE_2D);

	this->TextureIndex_ = (float)Data.TextureSlotIndex;
	Data.TextureSlots[Data.TextureSlotIndex] = textureID;
	Data.TextureSlotIndex++;

}

void Renderer::Text::Write(
	const std::string& text,
	glm::vec2 position,
	glm::vec2 size,
	const glm::vec4& color,
	HorizontalAlignment horizontalAlignment,
	VerticalAlignment verticalAlignment) {

	size /= glm::vec2(Height_);

	float maxTop = 0.0f, maxH = 0.0f;
	// Get the length of each line seperated by a "\n" for horizontal alignment
	std::vector<float> linesLength = { 0.0f };

	for (const auto& i : text) {
		if (i == '\n') {
			linesLength.push_back(0.0f);
			continue;
		}
		auto& c = Characters_[i];
		maxTop = std::max(maxTop, c.bitmapT);
		maxH = std::max(maxH, c.bitmapH);
		linesLength.at(linesLength.size() - 1) += c.advance.x * size.x;
	}

	auto totalHeight = linesLength.size() * maxH * 1.2;

	if (verticalAlignment == VerticalAlignment::Center) {
		position.y -= totalHeight * size.y / 2;
	}
	else if (verticalAlignment == VerticalAlignment::Top) {
		position.y -= totalHeight * size.y;
	}
	const auto originalPos = position;
	uint32_t line = 0;

	for (const auto& i : text) {
		if (i == '\n' || line == 0) {
			position.x = originalPos.x;

			if (horizontalAlignment == HorizontalAlignment::Center) {
				position.x -= linesLength.at(line) / 2;
			}
			else if (horizontalAlignment == HorizontalAlignment::Right) {
				position.x -= linesLength.at(line);
			}

			line++;
			if (i == '\n') {
				position.y += maxH * 1.2 * size.y;
				continue;
			}
		}
		if (Data.IndexCount >= MAX_INDEX_COUNT || Data.TextureSlotIndex > 31) {
			EndBatch();
			Flush();
			BeginBatch();
		}
		auto& c = Characters_[i];
		auto x = position.x + c.bitmapL * size.x;
		auto y = position.y + (maxH - c.bitmapT) * size.y;

		Data.BufferPtr->Position = { x, y };
		Data.BufferPtr->Color = color;
		Data.BufferPtr->TexCoords = { c.textureOffset.x ,c.textureOffset.y };
		Data.BufferPtr->TexIndex = this->TextureIndex_;
		Data.BufferPtr->IsText = 1.0f;
		Data.BufferPtr++;

		Data.BufferPtr->Position = { x + c.bitmapW * size.x, y };
		Data.BufferPtr->Color = color;
		Data.BufferPtr->TexCoords = { c.textureOffset.x + c.bitmapW / (float)TextureWidth_ ,c.textureOffset.y };
		Data.BufferPtr->TexIndex = this->TextureIndex_;
		Data.BufferPtr->IsText = 1.0f;
		Data.BufferPtr++;

		Data.BufferPtr->Position = { x + c.bitmapW * size.x, y + c.bitmapH * size.y };
		Data.BufferPtr->Color = color;
		Data.BufferPtr->TexCoords = { c.textureOffset.x + c.bitmapW / (float)TextureWidth_ ,c.textureOffset.y + c.bitmapH / (float)TextureHeight_ };
		Data.BufferPtr->TexIndex = this->TextureIndex_;
		Data.BufferPtr->IsText = 1.0f;
		Data.BufferPtr++;

		Data.BufferPtr->Position = { x, y + c.bitmapH * size.y };
		Data.BufferPtr->Color = color;
		Data.BufferPtr->TexCoords = { c.textureOffset.x, c.textureOffset.y + c.bitmapH / (float)TextureHeight_ };
		Data.BufferPtr->TexIndex = this->TextureIndex_;
		Data.BufferPtr->IsText = 1.0f;
		Data.BufferPtr++;

		Data.IndexCount += 6;
		position += c.advance * size;
	}
}
