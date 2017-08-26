#pragma once

#include <GL\glew.h>
#include <glm\gtc\matrix_transform.hpp>

using glm::vec3;

enum Direction
{
	FORWARD, BACKWARD, LEFT, RIGHT
};

class Camera
{
private:
	vec3 pos;
	vec3 front;
	vec3 right;
	vec3 up;
	vec3 worldUp;
	GLfloat pitch;
	GLfloat yaw;
	GLfloat moveSpeed;
	GLfloat mouseSens;
	void updateVectors();
public:
	Camera(vec3 newPos = vec3(0.0f, 0.0f, 3.0f), vec3 newWorldUp = vec3(0.0f, 1.0f, 0.0f), GLfloat newPitch = 0.0f, GLfloat newYaw = -90.0f, GLfloat newMoveSpeed = 3.0f, GLfloat newMouseSens = 0.1f);
	glm::mat4 getViewMatrix();
	void processKeyboard(Direction dir, GLfloat deltaT);
	void processMouseMove(GLfloat xoffset, GLfloat yoffset);
};