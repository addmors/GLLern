#include "Camera.h"

Camera::Camera(glm::vec3 Pos = glm::vec3(3.0f, 0.0f, 3.0f), glm::vec3 Front = glm::vec3(-1.0f, -0.0f, -0.0f), glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat fov = 45.0f){
	this->objPos = glm::vec3(0);
	this->cameraPos = Pos;
	this->lengthtoobj = glm::length(Pos);
	this->cameraFront = glm::normalize(-Pos);
	this->cameraUp = Up;
	this->fov = fov;
	this->pitch = glm::degrees(glm::asin(cameraFront.y));
	//V TLK0
	//this->yaw = yaw;
	this->yaw = glm::degrees(glm::atan(cameraFront.z , cameraFront.x));
	this->lastX = 400;
	this->lastY = 300;
}
void Camera::mouse_callback(double xpos, double ypos) {

	
	cameraPos = objPos - cameraFront * glm::length(lengthtoobj);
	cameraPos.y += 1.8;
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
	
}
void Camera::scroll_callback (double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}
void Camera::do_movement()
{
	// Camera controls
	//GLfloat cameraSpeed = 5.0f * deltaTime;
	/*if (keys->at(87))
		objPos += cameraSpeed * cameraFront;
		
	if (keys->at(83))
		objPos -= cameraSpeed * cameraFront;
	if (keys->at(65))
		objPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys->at(68))
		objPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys->at(81))
		objPos += cameraSpeed * cameraUp;
	if(keys->at(69))
		objPos -= cameraSpeed * cameraUp;
	objPos.y = 0;*/
	cameraFront = glm::normalize(front);
	cameraPos = objPos - cameraFront * glm::length(lengthtoobj);
	cameraPos.y += 1.8;
}
glm::mat4 faseLookAt() {}
glm::mat4 Camera::LoocAt() {
	
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->cameraFront = glm::normalize(front);
	return glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
}

glm::vec3 Camera::cameraRight()
{
	return glm::normalize(glm::cross(cameraFront, cameraUp));//123
}
