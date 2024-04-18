#include "Camera.h"

#include <gtc/matrix_transform.hpp>

const float Camera::Z_NEAR = 0.1f;
const float Camera::Z_FAR = 500.0f;
const float Camera::YAW = -90.0f;
const float Camera::PITCH = 0.0f;
const float Camera::FOV = 45.0f;
const float Camera::SPEED_FACTOR = 10.0f;
const float Camera::MOUSE_SENSITIVITY = 0.05f;
const float Camera::SPEED_BOOST_MULTIPLIER = 0.1f;
const glm::vec3 Camera::START_POSITION = glm::vec3(0.0f, 0.0f, 10.0f);
const glm::vec3 Camera::START_FACING = glm::vec3(0.0f, 0.0f, -1.0f);

Camera::Camera(int width, int height, const glm::vec3& position)
{
	Set(width, height, position);
}

void Camera::Set(int width, int height, const glm::vec3& position)
{
	isPerspective = true;
	yaw = Camera::YAW;
	pitch = Camera::PITCH;

	fovY = Camera::FOV;
	this->width = width;
	this->height = height;
	zNear = Camera::Z_NEAR;
	zFar = Camera::Z_FAR;

	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->position = position;

	lastX = width / 2.0f;
	lastY = height / 2.0f;
	isFirstMouseMove = true;

	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(position, position + forward, up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	if (isPerspective)
	{
		float aspectRatio = ((float)(width)) / height;
		return glm::perspective(glm::radians(fovY), aspectRatio, zNear, zFar);
	}

	float scaleFactor = 2000.0f;
	return glm::ortho(
		-width / scaleFactor, width / scaleFactor,
		-height / scaleFactor, height / scaleFactor, -zFar, zFar);
}

glm::vec3 Camera::GetPosition() const
{
	return position;
}

void Camera::MoveCamera(float xOffset, float yOffset, float zOffset)
{
	position += xOffset * right * Camera::SPEED_FACTOR;
	position += yOffset * up * Camera::SPEED_FACTOR;
	position += zOffset * forward * Camera::SPEED_FACTOR;
}

void Camera::MoveForward(float distance)
{
	MoveCamera(0, 0, distance);
}

void Camera::MoveBackward(float distance)
{
	MoveCamera(0, 0, -distance);
}

void Camera::MoveRight(float distance)
{
	MoveCamera(distance, 0, 0);
}

void Camera::MoveLeft(float distance)
{
	MoveCamera(-distance, 0, 0);
}

void Camera::MoveUp(float distance)
{
	MoveCamera(0, distance, 0);
}

void Camera::MoveDown(float distance)
{
	MoveCamera(0, -distance, 0);
}

void Camera::HandlMouseMovement(float deltaX, float deltaY)
{
	ProcessMouseMovement((deltaX - lastX) * MOUSE_SENSITIVITY, (lastY - deltaY) * MOUSE_SENSITIVITY);
	lastX = deltaX;
	lastY = deltaY;
	UpdateCameraVectors();
}

void Camera::HandleMouseScroll(float yOffset)
{
	ProcessMouseScroll(yOffset);
	UpdateCameraVectors();
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
	if (isFirstMouseMove)
	{
		isFirstMouseMove = false;
		return;
	}

	yaw += xOffset;
	pitch += yOffset;

	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		else if (pitch < -89.0f)
			pitch = -89.0f;
	}
}

void Camera::ProcessMouseScroll(float yOffset)
{
	fovY -= yOffset;

	if (fovY <= 1.0f)
		fovY = 1.0f;
	else if (fovY >= 45.0f)
		fovY = 45.0f;
}

void Camera::UpdateCameraVectors()
{
	this->forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->forward.y = sin(glm::radians(pitch));
	this->forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	this->forward = glm::normalize(this->forward);
	this->right = glm::normalize(glm::cross(forward, worldUp));
	this->up = glm::normalize(glm::cross(right, forward));
}