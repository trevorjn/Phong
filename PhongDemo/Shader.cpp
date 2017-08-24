#include <fstream>
#include <sstream>
#include <iostream>

#define GLEW_STATIC
#include <GL\glew.h>

#include "Shader.h"

const GLint INFO_LOG_SIZE = 512;

Shader::Shader(const GLchar* vert, const GLchar* frag)
{
	// Source code of vertex and fragment shaders
	const GLchar* vertCode = readShaderFile(vert, "vertex");
	const GLchar* fragCode = readShaderFile(frag, "fragment");

	// Define these variables for checking compile and link status of shaders/program
	GLint success;
	GLchar infoLog[INFO_LOG_SIZE];

	// Create, source, and compile vertex shader
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertCode, nullptr);
	glCompileShader(vertShader);
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertShader, INFO_LOG_SIZE, nullptr, infoLog);
		std::cout << "ERROR: failed to compile vertex shader\n" << infoLog << std::endl;
	}

	// Create, source, and compile fragment shader
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragCode, nullptr);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragShader, INFO_LOG_SIZE, nullptr, infoLog);
		std::cout << "ERROR: failed to compile fragment shader\n" << infoLog << std::endl;
	}

	// Create shader program, attach vertex and fragment shaders, and link the program
	ID = glCreateProgram();
	glAttachShader(ID, vertShader);
	glAttachShader(ID, fragShader);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, INFO_LOG_SIZE, nullptr, infoLog);
		std::cout << "ERROR: failed to link shader program\n" << infoLog << std::endl;
	}

	// Delete vertex and fragment shaders now that they have been linked
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
}

const char* Shader::readShaderFile(const GLchar* fileName, const GLchar* shaderType)
{
	std::ifstream file;
	std::string code;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		file.open(fileName);
		std::stringstream stream;
		stream << file.rdbuf();
		file.close();
		code = stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR: failed to open " << shaderType << " shader source file" << std::endl;
	}
	return code.c_str();
}