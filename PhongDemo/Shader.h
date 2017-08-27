#pragma once

#include <string>
#include <memory>

#include <GL\glew.h>
#include <glm\gtc\matrix_transform.hpp>

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;
};

class Shader
{
private:
	GLuint ID;
	std::string readShaderFile(const GLchar* fileName, const GLchar* shaderType);
public:
	Shader(const GLchar* vert, const GLchar* frag);
	void use() const;
	void setFloat(const GLchar* name, GLfloat value) const;
	void setVec3(const GLchar* name,  glm::vec3 value) const;
	void setVec3(const GLchar* name, GLfloat v1, GLfloat v2, GLfloat v3);
	void setMat4(const GLchar* name, glm::mat4 value) const;
	void setMaterial(const GLchar* name, Material mat);
};