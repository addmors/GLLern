#include "Camera.h"

Camera::Camera(glm::vec3 Pos, glm::vec3 Front, glm::vec3 Up, GLfloat fov, GLfloat yaw, GLfloat pitch){
	this->cameraPos = Pos;
	this->cameraFront = glm::normalize(Front);
	this->cameraUp = Up;
	this->fov = fov;
	this->pitch = glm::degrees(glm::atan(Front.y,-Front.z));
	//this->yaw = yaw;
	this->yaw = glm::degrees(glm::atan(Front.z , Front.x));
	this->lastX = 400;
	this->lastY = 300;
}
void Camera::key_callback(int key, int action)
{
	// Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
	// и приложение после этого закроется
	if (action == 1) {
		keys[key] = true;
	}
	else if (action == 0)
		keys[key] = false;
}
void Camera::mouse_callback(double xpos, double ypos) {

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Обратный порядок вычитания потому что оконные Y-координаты возрастают с верху вниз 
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}
void Camera::scroll_callback (double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
	std::cout << fov << " ";
}
void Camera::do_movement()
{
	// Camera controls
	GLfloat cameraSpeed = 5.0f * deltaTime;
	if (keys[87])
		cameraPos += cameraSpeed * cameraFront;
	if (keys[83])
		cameraPos -= cameraSpeed * cameraFront;
	if (keys[65])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[68])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[81])
		cameraPos += cameraSpeed * cameraUp;
	if(keys[69])
		cameraPos -= cameraSpeed * cameraUp;
}glm::mat4 Camera::LoocAt() {
	return glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
}