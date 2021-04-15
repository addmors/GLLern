#pragma once
#include <vector>
#include <string>
#include "../shader/Shad.h"
#include "../Mesh/Mesh.h"
#include<SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "../Bone/Bone.h"
#include "../AniNode/AnimNode.h"
#include "../Animations/Animations.h"
using namespace std;

class Model
{
public:
	/*  Методы   */
	std::vector<Animation> animations;
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	Animation Animations{ "idle",glm::vec2(0,0),0};
	Animation* CurrAnimation;
	vector<Animation> Anims;
	string directory;
	vector <Bone> bones;
	std::vector<aiNode*> ai_nodes;
	std::vector<aiNodeAnim*> ai_nodes_anim; 
	bool anim = false;
	Skeleton skel;
	Animation* FindAnimation(std::string anim_to_find);
	glm::mat4 globalInverseTransform;
	bool gammaCorrection;
	GLfloat deltatime;
	void key_callback(int, int);
	void PlayAnimation(float);
	Model(bool gamma = false) : gammaCorrection(gamma) {};

	Model(string const &path, bool gamma = false) : gammaCorrection(gamma)
	{
		loadModel(path);
	}
	void loadAnims(string path, string name);
	
	void loadIdleAnimaitons(string path);
	void DrawAnim(Shader shader, glm::mat4 model, glm::mat4 wiew, glm::mat4 projection);
	void Draw(Shader shader);
	void AnimNodeProcess();
	aiScene* scene;

	void loadModel(string path);
private:
	/*  Данные модели  */

	/*  Методы   */
	Bone* FindBone(std::string name);
	aiNode* FindAiNode(std::string name);
	aiNodeAnim* FindAiNodeAnim(std::string name);
	int FindBoneIDByName(std::string name);
	void recursiveNodeProcess(aiNode* node);
	void processNode(aiNode *node, const aiScene *scene, aiMatrix4x4 transformation);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene, aiMatrix4x4 transformation);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
	bool keys[1024];
};

