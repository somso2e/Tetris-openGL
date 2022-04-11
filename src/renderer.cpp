#include "renderer.hpp"


void Renderer::Init2D() {
	if (s_Data.Initilized)
		return;
	s_Data.Initilized = true;
	// Initilize the shader
	s_Data.QuadShader.Compile("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
	s_Data.QuadShader.Use();
	GLint samplers[MAX_TEXTURE_COUNT];
	for (int i = 0; i < MAX_TEXTURE_COUNT; i++) {
		samplers[i] = i;
	}
	s_Data.QuadShader.SetUniform1iv("uTextures", MAX_TEXTURE_COUNT, samplers);

	// Set vertex attributes
	s_Data.Buffer = new Vertex[MAX_VERTEX_COUNT];

	glCreateVertexArrays(1, &s_Data.VAO);
	glBindVertexArray(s_Data.VAO);

	glCreateBuffers(1, &s_Data.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_COUNT * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexArrayAttrib(s_Data.VAO, 0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Vertex::Position));

	glEnableVertexArrayAttrib(s_Data.VAO, 1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Vertex::Color));

	glEnableVertexArrayAttrib(s_Data.VAO, 2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Vertex::TexCoords));

	glEnableVertexArrayAttrib(s_Data.VAO, 3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Vertex::TexIndex));

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
	glCreateBuffers(1, &s_Data.IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Set default white texture 
	glCreateTextures(GL_TEXTURE_2D, 1, &s_Data.WhiteTextureID);
	glBindTexture(GL_TEXTURE_2D, s_Data.WhiteTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	uint32_t whiteColor = 0xFFFFFFFF;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &whiteColor);

	s_Data.TextureSlots[0] = s_Data.WhiteTextureID;
}


void Renderer::Shutdown() {
	glDeleteVertexArrays(1, &s_Data.VAO);
	glDeleteBuffers(1, &s_Data.VBO);
	glDeleteBuffers(1, &s_Data.IBO);

	for (auto i = 0; i < MAX_TEXTURE_COUNT; i++)
		glDeleteTextures(1, &s_Data.TextureSlots[i]);

	delete[] s_Data.Buffer;
	s_Data.QuadShader.Delete();
}


void Renderer::BeginBatch() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	s_Data.QuadShader.Use();
	s_Data.BufferPtr = s_Data.Buffer;
}


void Renderer::EndBatch() {
	const glm::mat4 projection = glm::ortho(
		0.0f, static_cast<float> (WINDOW_WIDTH),
		static_cast<float> (WINDOW_HEIGHT), 0.0f,
		0.0f, 1.0f);
	s_Data.QuadShader.SetMatrix4("projection", projection);

	glm::mat4 model = glm::mat4(1.0f);
	s_Data.QuadShader.SetMatrix4("model", model);


	GLsizeiptr size = (uint8_t*)s_Data.BufferPtr - (uint8_t*)s_Data.Buffer;
	glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.Buffer);
}


void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2 size, const glm::vec4& color, uint32_t textureID) {
	if (s_Data.IndexCount >= MAX_INDEX_COUNT || s_Data.TextureSlotIndex > 31) {
		EndBatch();
		Flush();
		BeginBatch();
	}

	float textureIndex = 0.0f;
	for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
		if (s_Data.TextureSlots[i] == textureID) {
			textureIndex = (float)i;
		}
	}
	if (textureIndex == 0.0f) {
		textureIndex = (float)s_Data.TextureSlotIndex;
		s_Data.TextureSlots[s_Data.TextureSlotIndex] = textureID;
		s_Data.TextureSlotIndex++;
	}

	s_Data.BufferPtr->Position = { position.x, position.y };
	s_Data.BufferPtr->Color = color;
	s_Data.BufferPtr->TexCoords = { 0.0f,0.0f };
	s_Data.BufferPtr->TexIndex = textureIndex;
	s_Data.BufferPtr++;

	s_Data.BufferPtr->Position = { position.x + size.x, position.y };
	s_Data.BufferPtr->Color = color;
	s_Data.BufferPtr->TexCoords = { 1.0f,0.0f };
	s_Data.BufferPtr->TexIndex = textureIndex;
	s_Data.BufferPtr++;

	s_Data.BufferPtr->Position = { position.x + size.x, position.y + size.y };
	s_Data.BufferPtr->Color = color;
	s_Data.BufferPtr->TexCoords = { 1.0f,1.0f };
	s_Data.BufferPtr->TexIndex = textureIndex;
	s_Data.BufferPtr++;

	s_Data.BufferPtr->Position = { position.x, position.y + size.y };
	s_Data.BufferPtr->Color = color;
	s_Data.BufferPtr->TexCoords = { 0.0f,1.0f };
	s_Data.BufferPtr->TexIndex = textureIndex;
	s_Data.BufferPtr++;

	s_Data.IndexCount += 6;
}


GLuint Renderer::LoadTexture(const char* imageFilePath) {

	GLuint ID;
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);

	// is numColCh needed????
	unsigned char* bytes = stbi_load(imageFilePath, &widthImg, &heightImg, &numColCh, 0);
	if (!bytes) {
		std::cerr << "[ERROR](Texture) Image File " << imageFilePath << " Could not be loaded." << std::endl;
	}
	glCreateTextures(GL_TEXTURE_2D, 1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	return ID;
}

void Renderer::Flush() {
	for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
		glBindTextureUnit(i, s_Data.TextureSlots[i]);
	}
	glBindVertexArray(s_Data.VAO);
	glDrawElements(GL_TRIANGLES, s_Data.IndexCount, GL_UNSIGNED_INT, nullptr);
	s_Data.IndexCount = 0;
}

///////////////////////////
FT_Face Renderer::InitText(const char* fontFilePath) {
	if (!s_Data.Initilized) {
		Renderer::Init2D();
	}
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cerr << "[ERROR](Text) Could not init FreeType Library" << std::endl;
	}
	FT_Face face;
	if (FT_New_Face(ft, fontFilePath, 0, &face)) {
		std::cerr << "[ERROR](Text) Failed to load font" << std::endl;
	}
	return face;
}

void Renderer::Atlas::Create(Font font, int height) {
	auto& face = font;

	FT_Set_Pixel_Sizes(face, 0, height);
	FT_GlyphSlot g = face->glyph;

	unsigned int roww = 0;
	unsigned int rowh = 0;

	unsigned int rowWidth = 0;
	unsigned int rowHeight = 0;

	memset(Characters, 0, sizeof Characters);
	std::cout << "asd";
	for (auto i = 32; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			std::cerr << "Loading character" << (char)i << "failed!\n";
			continue;
		}
		if (rowWidth + g->bitmap.width + 1 >= MAXWIDTH) {

		}
	}
	for (int i = 32; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			std::cerr << "Loading character" << (char)i << "failed!\n";
			continue;
		}
		rowWidth += g->bitmap.width + 1;
		rowHeight = std::max(rowHeight, g->bitmap.rows);

		if (rowWidth + g->bitmap.width + 1 >= MAXWIDTH) {
			textureWidth = std::max(textureWidth, rowWidth);
			textureHeight += rowWidth;
			rowWidth = 0;
			rowHeight = 0;
		}
	}

	textureWidth = std::max(textureWidth, rowWidth);
	textureHeight += rowHeight;

	glCreateTextures(GL_TEXTURE_2D, 1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, textureWidth, textureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	int offsetX = 0;
	int offsetY = 0;

	rowh = 0;

	for (int i = 32; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			std::cerr << "Loading character" << i << "failed!\n";
			continue;
		}

		if (offsetX + g->bitmap.width + 1 >= MAXWIDTH) {
			offsetY += rowh;
			rowh = 0;
			offsetX = 0;
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
		Characters[i].advance.x = (float)(g->advance.x >> 6);
		Characters[i].advance.y = (float)(g->advance.y >> 6);

		Characters[i].bitmapW = (float)g->bitmap.width;
		Characters[i].bitmapH = (float)g->bitmap.rows;

		Characters[i].bitmapL = (float)g->bitmap_left;
		Characters[i].bitmapT = (float)g->bitmap_top;

		Characters[i].textureOffset.x = offsetX / (float)textureHeight;
		Characters[i].textureOffset.y = offsetY / (float)textureWidth;

		rowh = std::max(rowh, g->bitmap.rows);
		offsetX += g->bitmap.width + 1;
	}
	//glPixelStorei(GL_PACK_ALIGNMENT, 1);

}

void Renderer::Atlas::Draw(std::string text, glm::vec2 position, glm::vec2 size, const glm::vec4& color) {
	float textureIndex = 0.0f;
	for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
		if (s_Data.TextureSlots[i] == this->TextureID) {
			textureIndex = (float)i;
		}
	}
	if (textureIndex == 0.0f) {
		textureIndex = (float)s_Data.TextureSlotIndex;
		s_Data.TextureSlots[s_Data.TextureSlotIndex] = this->TextureID;
		s_Data.TextureSlotIndex++;
	}

	for (const auto& i : text) {
		if (s_Data.IndexCount >= MAX_INDEX_COUNT || s_Data.TextureSlotIndex > 31) {
			EndBatch();
			Flush();
			BeginBatch();
		}
		auto c = Characters[i];
		auto x = position.x + c.bitmapL;
		auto y = position.y + c.bitmapT;
		s_Data.BufferPtr->Position = { x, y };
		s_Data.BufferPtr->Color = color;
		s_Data.BufferPtr->TexCoords = { c.textureOffset.x ,c.textureOffset.y };
		s_Data.BufferPtr->TexIndex = textureIndex;
		s_Data.BufferPtr++;

		s_Data.BufferPtr->Position = { x + c.bitmapW, y };
		s_Data.BufferPtr->Color = color;
		s_Data.BufferPtr->TexCoords = { c.textureOffset.x + c.bitmapW / textureWidth ,c.textureOffset.y };
		s_Data.BufferPtr->TexIndex = textureIndex;
		s_Data.BufferPtr++;

		s_Data.BufferPtr->Position = { x + c.bitmapW, y + c.bitmapH };
		s_Data.BufferPtr->Color = color;
		s_Data.BufferPtr->TexCoords = { c.textureOffset.x + c.bitmapW / textureWidth ,c.textureOffset.y + c.bitmapH / textureHeight };
		s_Data.BufferPtr->TexIndex = textureIndex;
		s_Data.BufferPtr++;

		s_Data.BufferPtr->Position = { x, y + c.bitmapH };
		s_Data.BufferPtr->Color = color;
		s_Data.BufferPtr->TexCoords = { c.textureOffset.x, c.textureOffset.y + c.bitmapH / textureHeight };
		s_Data.BufferPtr->TexIndex = textureIndex;
		s_Data.BufferPtr++;

		s_Data.IndexCount += 6;

		position += c.advance;
	}

}
