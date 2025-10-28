#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float c_YAW = -90.0f;
const float c_PITCH = 0.0f;
const float c_SPEED = 2.5f;
const float c_SENSITIVITY = 0.1f;
const float c_ZOOM = 45.0f;

class Camera
{
public:
	Camera();
	~Camera();

	glm::mat4 getViewMatrix();
	void processMouse(float x, float y);
	void processKeyboard(Camera_Movement direction, float deltaTime);

private:
	glm::vec3 m_position;
	//z axis
	glm::vec3 m_front;
	//x axis
	glm::vec3 m_right;
	//y axis
	glm::vec3 m_up;
	glm::vec3 m_upVector;

	//camera rotations
	float m_yaw;
	float m_pitch;
	float m_movementSpeed;
	float m_mouseSensitivity;
	float m_zoom;

	void updateCameraVectors();
};