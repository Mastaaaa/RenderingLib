#include "camera.h"

Camera::Camera()
	: cameraPosition_(0.0f, 0.0f, 3.0f)
	, cameraTarget_(0.0f, 0.0f, -1.0f)
	, cameraDirection_(glm::normalize(cameraTarget_ - cameraPosition_))
	, upVector_(0.0f, 1.0f, 0.0f)
	, cameraRight_(glm::normalize(glm::cross(upVector_, cameraDirection_)))
	, cameraUp_(glm::normalize(glm::cross(cameraRight_, cameraDirection_)))
{
	updateCameraView();
}

Camera::~Camera()
{
	//nothing
}

glm::mat4 Camera::updateCameraView()
{
	cameraView_ = glm::lookAt(cameraPosition_, cameraPosition_ + cameraDirection_, upVector_);
	return cameraView_;
}

void Camera::processMouse(float x, float y)
{
	//sens must stay inside?
	float sens = 0.1f;
	x *= sens;
	y *= sens;

	yaw_ += x;
	pitch_ -= y;

	if (pitch_ > 89.0f)
	{
		pitch_ = 89.0f;
	}
	if (pitch_ < -89.0f)
	{
		pitch_ = -89.0f;
	}
	glm::vec3 front;
	front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front.y = sin(glm::radians(pitch_));
	front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	cameraDirection_ = glm::normalize(front);
}

void Camera::setCameraPosition(glm::vec3 position)
{
	cameraPosition_ = position;
}

void Camera::incrementCameraPosition(glm::vec3 position)
{
	cameraPosition_ += position;
}
