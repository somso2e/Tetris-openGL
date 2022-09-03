#include "application.hpp"

Game Application::Tetris;
GLFWwindow* Application::window;


int Application::Run() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif // _DEBUG

	if (!glfwInit()) {
		log("GLFW initialization failed");
		return -1;
	}
	else {
		log("GLFW initialized");
	}

	window = glfwCreateWindow(windowWidth, windowHeight, "Tetris", NULL, NULL);

	SetIcon("res/textures/icon2.png");

	glfwSetWindowAspectRatio(window, 16, 9);
	glfwSetWindowSizeLimits(window, 1280, 720, GLFW_DONT_CARE, GLFW_DONT_CARE);

	if (!window) {
		glfwTerminate();
		log("Window context creation failed");
		return -1;
	}
	else {
		log("Window created successfully");
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, windowWidth, windowHeight);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef _DEBUG
	glDebugMessageCallback(MessageCallback, 0);
#endif // _DEBUG


	Tetris.Init();

	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(window)) {
		Tetris.Update();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Application::Shutdown() {
	Renderer::Shutdown();
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Application::SetIcon(const char* iconFilePath) {
	GLFWimage iconImage[1];

	iconImage[0].pixels = stbi_load(iconFilePath, &iconImage->width, &iconImage->height, 0, 4);
	if (!iconImage[0].pixels) {
		log("[ERROR](Texture) Icon file " + std::string(iconFilePath) + " could not be loaded.");
	}
	glfwSetWindowIcon(window, 1, iconImage);
	stbi_image_free(iconImage[0].pixels);

}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (0 < key && key < 350) {
		if (key == GLFW_KEY_F12 && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		Tetris.Keys_[key] = action;
		if (action == GLFW_RELEASE) {
			Tetris.KeysProcessed_[key] = false;
		}
	}
}


void Application::window_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
