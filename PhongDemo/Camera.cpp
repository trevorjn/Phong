#include "Camera.h"

Camera::Camera(vec3 newPos, vec3 newWorldUp, GLfloat newPitch, GLfloat newYaw, GLfloat newMoveSpeed, GLfloat newMouseSens)
	: pos(newPos), worldUp(newWorldUp), pitch(newPitch), yaw(newYaw), moveSpeed(newMoveSpeed), mouseSens(newMouseSens)
{
	updateVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(pos, pos + front, up);
}

void Camera::updateVectors()
{
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::processKeyboard(Direction dir, GLfloat deltaT)
{
	GLfloat scaledSpeed = moveSpeed * deltaT;
	if (dir == FORWARD)
	{
		pos += scaledSpeed * front;
	}
	if (dir == BACKWARD)
	{
		pos -= scaledSpeed * front;
	}
	if (dir == RIGHT)
	{
		pos += scaledSpeed * right;
	}
	if (dir == LEFT)
	{
		pos -= scaledSpeed * right;
	}
	if (dir == UP)
	{
		pos += scaledSpeed * worldUp;
	}
	if (dir == DOWN)
	{
		pos -= scaledSpeed * worldUp;
	}
}

void Camera::processMouseMove(GLfloat xoffset, GLfloat yoffset)
{
	pitch -= mouseSens * yoffset;
	yaw += mouseSens * xoffset;

	if (pitch > 89.9f)
	{
		pitch = 89.9f;
	}
	else if (pitch < -89.9f)
	{
		pitch = -89.9f;
	}

	updateVectors();
}