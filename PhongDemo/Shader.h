#pragma once

#include <string>
#include <GL\glew.h>

class Shader
{
private:
	GLuint ID;
	const char* readShaderFile(const GLchar* fileName, const GLchar* shaderType);
public:
	Shader(const GLchar* vert, const GLchar* frag);
	void use() const;
	void setFloat() const;
	void setVec3() const;
	void setMat4() const;
};