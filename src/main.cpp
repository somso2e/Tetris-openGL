#define DEBUG
#include "cell.hpp"

int main() {

	////////////////////////////////
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	GLFWwindow* window;
	if (!glfwInit())
		return -1;
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(MessageCallback, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

	Shader shader;
	shader.Compile("res/shaders/texture.vertex", "res/shaders/texture.fragment");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float> (WINDOW_WIDTH), static_cast<float> (WINDOW_HEIGHT), 0.0f, 0.0f, 1.0f);
	//glm::mat4 projection = glm::ortho(0.0f, 10.0f, 10.0f, 0.0f, 0.0f, 1.0f);
	shader.SetMatrix4("projection", projection);

	Texture emptyCellTexture;
	emptyCellTexture.Generate("res/textures/EmptyCell.png");
	emptyCellTexture.texUnit(shader, "tex0", 0);


	Cell cell(shader, emptyCellTexture);
	std::vector <Color> colors{
	Color::Gray,
	Color::Yellow,
	Color::Green,
	Color::Orange,
	Color::Cyan,
	Color::Magenta,
	Color::Pink,
	Color::Red };

	std::vector <std::string> colorsText{
	"Gray",
	"Yellow",
	"Green",
	"Orange",
	"Cyan",
	"Magenta",
	"Pink",
	"Red" };

	int colorIndex = 0;

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 20; j++) {

				glm::vec2 pos(CELL_WIDTH * i, CELL_HEIGHT * j);;
				std::cout << colorsText.at(colorIndex) << " " << colorIndex << std::endl;
				cell.Draw(pos, colors.at(colorIndex));
				//colorIndex++;
				colorIndex = ++colorIndex % 8;

				emptyCellTexture.Bind();
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	emptyCellTexture.Delete();
	shader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;

	////////////////////////////////////////
	/*
	std::vector <Vertex> vertices = {
		Vertex{glm::vec2(-1.0f, -1.0f),  glm::vec2(0.0f, 0.0f)},
		Vertex{glm::vec2(-1.0f,  1.0f),  glm::vec2(0.0f, 1.0f)},
		Vertex{glm::vec2(1.0f,   1.0f),  glm::vec2(1.0f, 1.0f)},
		Vertex{glm::vec2(1.0f,  -1.0f),  glm::vec2(1.0f, 0.0f)}
	};
	std::vector<GLuint> indices = {
	0, 2, 1,
	0, 3, 2
	};

	VAO VAO1;
	VAO1.Bind();
	VBO VBO1(vertices);
	IBO IBO1(indices);
	VAO1.LinkAttrib(VBO1, 0, 2, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(2 * sizeof(float)));
	VBO1.Bind();
	IBO1.Bind();

	Shader shader;
	shader.Compile("res/shaders/texture.vertex", "res/shaders/texture.fragment");

	glm::mat4 projection = glm::ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f);
	//shader.SetMatrix4("projection", projection);

	Texture emptyCellTexture;
	emptyCellTexture.Generate("res/textures/EmptyCell.png");
	emptyCellTexture.texUnit(shader, "tex0", 0);


	glm::vec4 colorCode(1.0f, 0.0f, 0.0f, 1.0f);
	glUniform4f(glGetUniformLocation(shader.ID, "aColor"), colorCode.x, colorCode.y, colorCode.z, colorCode.w);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		VAO1.Bind();
		emptyCellTexture.Bind();
		//cell1.Draw(glm::vec2(10, 10), glm::vec2(10, 10), Color::Red);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	IBO1.Delete();
	emptyCellTexture.Delete();
	shader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
*/
}




///////////////////////////////////////
//std::vector<glm::uvec2> gridIndices;
//std::vector<glm::vec2> gridVertices;
//const auto gridW = 10;
//const auto gridH = 20;
//for (auto i = 0; i <= gridW; ++i) {
//	for (auto j = 0; j <= gridH; ++j) {
//		float x = -1 + 2 * (float)i / (float)gridW;
//		float y = -1 + 2 * (float)j / (float)gridH;
//		gridVertices.push_back(glm::vec2(x, y));
//
//	}
//}
//int gridDim = gridH + 1;
//for (int j = 0; j <= gridH; j++) {
//	gridIndices.push_back(glm::uvec2(j * gridDim, (j + 1) * gridDim - 1));
//	gridIndices.push_back(glm::uvec2(j, (gridVertices.size() - gridDim) + j));
//}
//std::vector<glm::uvec3> cellsIndices;
//cellsIndices.push_back(glm::uvec3(0, 1, 10));
//cellsIndices.push_back(glm::uvec3(1, 10, 11));