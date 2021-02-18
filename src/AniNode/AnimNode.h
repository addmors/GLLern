#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class AnimNode{
public:
	AnimNode(aiNode* nd, aiNodeAnim* anim) : node(nd), animNode(anim) {};

	aiNode* node;
	aiNodeAnim* animNode;
	glm::vec3 pos;
	glm::quat rot;
	glm::vec3 scale;
	glm::vec3 p1;
	glm::vec3 p2;

	unsigned int FindPosition(float time);
	glm::vec3 CalcInterpolatedPosition(float time);
	unsigned int FindScaling(float time);
	glm::vec3 CalcInterpolatedSkaling(float time);
	unsigned int FindRotation(float time);
	glm::quat CalcInterpolatedRotation(float time);

	void UpdateKeyframeTransform(float time);
};

