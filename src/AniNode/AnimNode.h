#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<tuple>

#include<vector>


class AnimNode{
public:
	AnimNode(aiNode* nd, aiNodeAnim* anim) : node(nd), animNode(anim),zeroPos(GetKeyframeTransform(0)) {
	};

	aiNode* node;
	aiNodeAnim* animNode;
	glm::vec3 pos;
	glm::quat rot;
	glm::vec3 scale;
	glm::mat4 zeroPos;
	glm::mat4 nowPos;

	unsigned int FindPosition(float time);
	glm::vec3 CalcInterpolatedPosition(float time);
	unsigned int FindScaling(float time);
	glm::vec3 CalcInterpolatedSkaling(float time);
	unsigned int FindRotation(float time);
	glm::quat CalcInterpolatedRotation(float time);
	void SLERP(float, float);
	void UpdateKeyframeTransform(float time);
	glm::mat4 GetKeyframeTransform(float time);
	glm::mat4 getZeroPos();
	glm::mat4 getNowPos();
};

