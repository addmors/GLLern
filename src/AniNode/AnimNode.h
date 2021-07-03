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
	aiMatrix4x4 zeroPos;
	aiMatrix4x4 nowPos;

	unsigned int FindPosition(float time);
	aiVector3D CalcInterpolatedPosition(float time);
	unsigned int FindScaling(float time);
	aiVector3D CalcInterpolatedSkaling(float time);
	unsigned int FindRotation(float time);
	aiQuaternion CalcInterpolatedRotation(float time);
	void UpdateKeyframeTransform(float time);
	aiMatrix4x4 GetKeyframeTransform(float time);
	aiMatrix4x4 getZeroPos();
	aiMatrix4x4 getNowPos();
};

