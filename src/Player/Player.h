#pragma once
#include "../Model/Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"../Camera/Camera.h"
#include<string>

#define MAX_SPEED 2

class Shader;

class Player
{	
public:
	Player(Model& model, Camera& _camera, Shader& _curshader) :Persone(&model), camera(&_camera),currentShader(&_curshader) {};
	Model* Persone = nullptr;
	Camera* camera = nullptr;
	Shader* currentShader = nullptr;
	glm::vec3 pos = {0.0,0.0,0.0};
	glm::vec3 direction = { 0.0,0.0,0.0 };
	glm::vec3 speeed = { 0.0,0.0,0.0 };
	glm::vec3 force = { 0.0,0.5,0.0};
	string name = "def_name";
	bool ongroud = 0, iscollidet  = 0;
	void update();
	void show(); 
};

