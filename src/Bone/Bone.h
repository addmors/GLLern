#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream> 
#include "../Animations/Animations.h"

class Mesh;
class Animation;
class GameObject;
class Skeleton;


static aiMatrix4x4 GLMMat4ToAi(glm::mat4 mat)
{
	return aiMatrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
		mat[1][0], mat[1][1], mat[1][2], mat[1][3],
		mat[2][0], mat[2][1], mat[2][2], mat[2][3],
		mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}

static glm::mat4 AiToGLMMat4(aiMatrix4x4& in_mat)
{
	glm::mat4 tmp;
	tmp[0][0] = in_mat.a1;
	tmp[1][0] = in_mat.b1;
	tmp[2][0] = in_mat.c1;
	tmp[3][0] = in_mat.d1;

	tmp[0][1] = in_mat.a2;
	tmp[1][1] = in_mat.b2;
	tmp[2][1] = in_mat.c2;
	tmp[3][1] = in_mat.d2;

	tmp[0][2] = in_mat.a3;
	tmp[1][2] = in_mat.b3;
	tmp[2][2] = in_mat.c3;
	tmp[3][2] = in_mat.d3;

	tmp[0][3] = in_mat.a4;
	tmp[1][3] = in_mat.b4;
	tmp[2][3] = in_mat.c4;
	tmp[3][3] = in_mat.d4;
	return tmp;
};

class Bone
{
public:
	Mesh* mesh;
	unsigned int id;
	std::string name;
	aiNode* node;
	aiNodeAnim* animNode = nullptr;
	Bone* parent_bone = nullptr;
	glm::mat4 offset_matrix;
	glm::mat4 nowTransformation;


	void getTransform();

	Bone(Mesh* in_mesh, unsigned int in_id, std::string in_name, aiMatrix4x4 in_o_mat, aiNode* _node, aiNodeAnim* _animNode): 
	mesh(in_mesh),
		id(in_id),
	name(in_name),
	node(_node),
	animNode(_animNode),
	parent_bone(nullptr),
	offset_matrix(AiToGLMMat4(in_o_mat)){};

	Bone(Mesh* in_mesh, unsigned int in_id, std::string in_name, glm::mat4 in_o_mat, aiNode* _node, aiNodeAnim* _animNode) :
	mesh(in_mesh),
		id(in_id),
	name(in_name),
	node(_node),
	animNode(_animNode),
	parent_bone(nullptr),
	offset_matrix(in_o_mat) {};

	glm::mat4 GetParentTransforms();
};

