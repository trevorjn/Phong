#include <iostream>

#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Shader.h"
#include "Camera.h"

// Global constants
const GLint WINDOW_WIDTH = 1600;
const GLint WINDOW_HEIGHT = 900;
const GLchar* WINDOW_TITLE = "Phong Demo";

const GLchar* LIGHTING_VERTEX_SHADER = "lighting.vert";
const GLchar* LIGHTING_FRAGMENT_SHADER = "lighting.frag";

const GLfloat FOV = 45.0f;

// Function prototypes
void framebufferSizeCallback(GLFWwindow* window, GLint width, GLint height);
void cursorPosCallback(GLFWwindow* window, GLdouble xpos, GLdouble ypos);
void runRenderLoop(GLFWwindow* window, Shader& lightingShader);
GLFWwindow* createWindow();
void configureGLFW();
void processInput(GLFWwindow* window);

Camera cam;

GLfloat lastFrameTime = 0.0f;
GLfloat lastXPos = WINDOW_WIDTH / 2.0f;
GLfloat lastYPos = WINDOW_HEIGHT / 2.0f;

int main()
{
	// Initialize GLFW and create a window
	configureGLFW();
	GLFWwindow* window = createWindow();

	// Check if window was successfully created
	if (window == nullptr)
	{
		std::cout << "ERROR: failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	
	// Initialize GLEW to fetch OpenGL function pointers
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "ERROR: failed to initialize GLEW\n" << glewGetErrorString(err) << std::endl;
		return EXIT_FAILURE;
	}

	// Set initial viewport dimensions
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set callbacks
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);

	// Define a temporary XY-plane triangle for testing
	GLfloat vertices[] = {
		 0.0f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f
	};

	// Create a vertex array object and bind it to the OpenGL context
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create a vertex buffer object and bind it to the ARRAY_BUFFER target
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Buffer vertices to GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configure vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);	

	Shader lightingShader(LIGHTING_VERTEX_SHADER, LIGHTING_FRAGMENT_SHADER);

	// Loop until GLFW window is told to close
	lastFrameTime = glfwGetTime();
	glfwSetCursorPos(window, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
	runRenderLoop(window, lightingShader);

	// Destroy instance of GLFW and exit program
	glfwTerminate();
	return EXIT_SUCCESS;
}

void runRenderLoop(GLFWwindow* window, Shader& lightingShader)
{
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// Clear contents of window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 model;

		glm::mat4 view;
		view = cam.getViewMatrix();

		glm::mat4 projection;
		projection = glm::perspective(FOV, (GLfloat)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

		lightingShader.use();
		lightingShader.setMat4("model", model);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", projection);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap framebuffers and poll for events (keyboard/mouse input, window resizing, etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void processInput(GLFWwindow* window)
{
	GLfloat currentTime = glfwGetTime();
	GLfloat deltaT = currentTime - lastFrameTime;
	lastFrameTime = currentTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cam.processKeyboard(FORWARD, deltaT);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cam.processKeyboard(BACKWARD, deltaT);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cam.processKeyboard(LEFT, deltaT);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cam.processKeyboard(RIGHT, deltaT);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		cam.processKeyboard(UP, deltaT);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		cam.processKeyboard(DOWN, deltaT);
	}
}

GLFWwindow* createWindow()
{
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
	glfwMakeContextCurrent(window);
	return window;
}

void configureGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void cursorPosCallback(GLFWwindow* window, GLdouble xpos, GLdouble ypos)
{
	GLfloat xoffset = xpos - lastXPos;
	GLfloat yoffset = ypos - lastYPos;
	lastXPos = xpos;
	lastYPos = ypos;

	cam.processMouseMove(xoffset, yoffset);
}

void framebufferSizeCallback(GLFWwindow* window, GLint width, GLint height)
{
	glViewport(0, 0, width, height);
}