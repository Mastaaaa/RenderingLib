#include "camera.h"

Camera::Camera()
	: m_position(0.0f, 0.0f, 3.0f)
	, m_front(0.0f, 0.0f, -1.0f)
	, m_upVector(0.0f, 1.0f, 0.0f)
	, m_right(glm::normalize(glm::cross(m_upVector, m_front)))
	, m_up(glm::normalize(glm::cross(m_right, m_front)))
	, m_yaw(c_YAW)
	, m_pitch(c_PITCH)
	, m_movementSpeed(c_SPEED)
	, m_mouseSensitivity(c_SENSITIVITY)
	, m_zoom(c_ZOOM)
	, m_turbo(3.0f)
{

}

Camera::~Camera()
{
	//nothing
}

glm::mat4 Camera::getViewMatrix()
{
	return  glm::lookAt(m_position, m_position + m_front, m_up);
}

//it just update the front camera directions
void Camera::updateCameraVectors()
{
	m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front.y = sin(glm::radians(m_pitch));
	m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(m_front);

	m_right = glm::normalize(glm::cross(m_front, m_upVector));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::processMouse(float x, float y)
{
	x *= m_mouseSensitivity;
	y *= m_mouseSensitivity;

	m_yaw += x;
	m_pitch -= y;

	if (m_pitch > 89.0f)
	{
		m_pitch = 89.0f;
	}
	if (m_pitch < -89.0f)
	{
		m_pitch = -89.0f;
	}
	updateCameraVectors();
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime, bool isTurbo)
{

	float velocity = m_movementSpeed * deltaTime;
	if (isTurbo) velocity *= m_turbo;

	if (direction == FORWARD)
	{
		glm::vec3 flatFront = glm::normalize(glm::vec3(m_front.x, 0.0f, m_front.z));
		m_position += flatFront * velocity;
	}
	if (direction == BACKWARD)
	{
		glm::vec3 flatFront = glm::normalize(glm::vec3(m_front.x, 0.0f, m_front.z));
		m_position -= flatFront * velocity;
	}
	if (direction == RIGHT)
	{
		m_position += m_right * velocity;
	}
	if (direction == LEFT)
	{
		m_position -= m_right * velocity;
	}
	if (direction == UP)
	{
		m_position += m_up * velocity;
	}
	if (direction == DOWN)
	{
		m_position -= m_up * velocity;
	}
}



