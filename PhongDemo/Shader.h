#pragma once

#include <string>

#include <GL\glew.h>
#include <glm\gtc\matrix_transform.hpp>

class Shader
{
private:
	GLuint ID;
	const char* readShaderFile(const GLchar* fileName, const GLchar* shaderType);
public:
	Shader(const GLchar* vert, const GLchar* frag);
	void use() const;
	void setFloat(const char* name, GLfloat value) const;
	void setVec3(const char* name,  glm::vec3 value) const;
	void setVec3(const char* name, GLfloat v1, GLfloat v2, GLfloat v3);
	void setMat4(const char* name, glm::mat4 value) const;
};