#include "Camera.h"
#include <iostream>

Camera::Camera(vec3 newPos, vec3 newWorldUp, GLfloat newPitch, GLfloat newYaw, GLfloat newMoveSpeed, GLfloat newMouseSens, GLboolean newIsFocused, GLfloat newFov)
	: pos(newPos), worldUp(newWorldUp), pitch(newPitch), yaw(newYaw), moveSpeed(newMoveSpeed), mouseSens(newMouseSens), isFocused(newIsFocused), fov(newFov)
{
	updateVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(pos, pos + front, up);
}

glm::mat4 Camera::getProjectionMatrix(GLfloat aspectRatio, GLfloat near, GLfloat far)
{
	return glm::perspective(glm::radians(fov), aspectRatio, near, far);
}

void Camera::updateVectors()
{
	if (isFocused == GL_FALSE)
	{
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	}
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::processMouseScroll(GLfloat yoffset)
{
	GLfloat scrollSens = 1.0f;

	fov -= scrollSens * yoffset;

	// Constrain field of view to between 5 and 45 degrees
	if (fov > 45.0f)
	{
		fov = 45.0f;
	}
	if (fov < 1.0f)
	{
		fov = 1.0f;
	}
}

void Camera::processKeyboard(CameraAction action, GLfloat deltaT)
{
	GLfloat scaledSpeed = moveSpeed * deltaT;
	if (action == FORWARD)
	{
		moveCamera(front, scaledSpeed);
	}
	if (action == BACKWARD)
	{
		moveCamera(-front, scaledSpeed);
	}
	if (action == RIGHT)
	{
		moveCamera(right, scaledSpeed);
	}
	if (action == LEFT)
	{
		moveCamera(-right, scaledSpeed);
	}
	if (action == UP)
	{
		moveCamera(worldUp, scaledSpeed);
	}
	if (action == DOWN)
	{
		moveCamera(-worldUp, scaledSpeed);
	}
	if (action == FOCUS)
	{
		isFocused = !isFocused;
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

vec3 Camera::getPosition() const
{
	return pos;
}

void Camera::moveCamera(vec3 dir, GLfloat speed)
{
	if (isFocused == GL_TRUE)
	{
		vec3 tempFront = front;
		vec3 tempPos = pos;
		pos += dir * speed;
		vec3 posDiff = tempPos - pos;
		front = glm::normalize(tempFront + posDiff - pos);
	}
	else
	{
		pos += dir * speed;
	}
}