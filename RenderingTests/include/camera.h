#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Camera
{
public:
	Camera();
	~Camera();
	glm::mat4 cameraView_;
	glm::mat4 updateCameraView();
	void setCameraPosition(glm::vec3 position);
	void incrementCameraPosition(glm::vec3 position);
	void processMouse(float x, float y);

	glm::vec3 cameraPosition_;
	glm::vec3 cameraTarget_;
	//z axis
	glm::vec3 cameraDirection_;
	glm::vec3 upVector_;
	//x axis
	glm::vec3 cameraRight_;
	//y axis
	glm::vec3 cameraUp_;

	//camera rotations
	float yaw_;
	float pitch_;

private:

};