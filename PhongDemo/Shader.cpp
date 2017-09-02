#include <fstream>
#include <sstream>
#include <iostream>

#define GLEW_STATIC
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.h"

const GLint INFO_LOG_SIZE = 512;

Shader::Shader(const GLchar* vert, const GLchar* frag)
{
	// Source code of vertex and fragment shaders
	std::string vertex = readShaderFile(vert, "vertex");
	std::string fragment = readShaderFile(frag, "fragment");
	const char* vertCode = vertex.c_str();
	const char* fragCode = fragment.c_str();

	// Declare these variables for checking compile and link status of shaders/program
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

void Shader::use() const
{
	glUseProgram(ID);
}

void Shader::setFloat(const char* name, GLfloat value) const
{
	GLuint loc = glGetUniformLocation(ID, name);
	glUniform1f(loc, value);
}

void Shader::setVec3(const char* name, glm::vec3 value) const
{
	GLuint loc = glGetUniformLocation(ID, name);
	glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Shader::setVec3(const char* name, GLfloat v1, GLfloat v2, GLfloat v3)
{
	glm::vec3 value(v1, v2, v3);
	GLuint loc = glGetUniformLocation(ID, name);
	glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Shader::setMat4(const char* name, glm::mat4 value) const
{
	GLuint loc = glGetUniformLocation(ID, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUInt(const char* name, GLuint value) const
{
	GLuint loc = glGetUniformLocation(ID, name);
	glUniform1ui(loc, value);
}

void Shader::setMaterial(const GLchar* name, Material mat)
{
	std::string diffuseName = name;
	diffuseName.append(".diffuse");
	setUInt(diffuseName.c_str(), mat.diffuse);

	std::string specularName = name;
	specularName.append(".specular");
	setVec3(specularName.c_str(), mat.specular);

	std::string shininessName = name;
	shininessName.append(".shininess");
	setFloat(shininessName.c_str(), mat.shininess);
}

std::string Shader::readShaderFile(const GLchar* fileName, const GLchar* shaderType)
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
	return code;
}