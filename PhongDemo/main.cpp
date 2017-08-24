#define GLEW_STATIC

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>

using std::cout;
using std::endl;

const GLint WINDOW_WIDTH = 1600;
const GLint WINDOW_HEIGHT = 900;

const char* WINDOW_TITLE = "Phong Demo";

int main()
{
	// Initialize GLFW for windowing and event handling
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFW window
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
	if (window == nullptr)
	{
		cout << "ERROR: failed to create GLFW window" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW to fetch OpenGL function pointers
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		cout << "ERROR: failed to initialize GLEW\n" << glewGetErrorString(err) << endl;
		return EXIT_FAILURE;
	}

	// Set initial viewport dimensions
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Clear contents of window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Swap framebuffers and poll for events (keyboard/mouse input, window resizing, etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}