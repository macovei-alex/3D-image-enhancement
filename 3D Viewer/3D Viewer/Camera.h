#pragma once

#include <glm.hpp>

class Camera
{
public:
	Camera(int width, int height, const glm::vec3& position = START_POSITION);
	void Set(int width, int height, const glm::vec3& position = START_POSITION);

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	glm::vec3 GetPosition() const;

	void MoveCamera(float xOffset, float yOffset, float zOffset);

	void MoveForward(float distance);
	void MoveBackward(float distance);
	void MoveRight(float distance);
	void MoveLeft(float distance);
	void MoveUp(float distance);
	void MoveDown(float distance);

	void HandlMouseMovement(float xPos, float yPos);
	void HandleMouseScroll(float yOffset);

private:
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void ProcessMouseScroll(float yOffset);
	void UpdateCameraVectors();

protected:
	float zNear, zFar, fovY;
	int width, height;
	bool isPerspective;

	glm::vec3 position, forward, right, up, worldUp;

	float yaw, pitch;

	bool isFirstMouseMove = true;
	float lastX = 0.0f, lastY = 0.0f;

public:
	static const float Z_NEAR;
	static const float Z_FAR;
	static const float YAW;
	static const float PITCH;
	static const float FOV;
	static const float SPEED_FACTOR;
	static const float MOUSE_SENSITIVITY;
	static const float SPEED_BOOST_MULTIPLIER;
	static const glm::vec3 START_POSITION;
	static const glm::vec3 START_FACING;
};