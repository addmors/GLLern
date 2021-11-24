#include "Model.h"

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

void Model::loadAnims(string path, string name)
{
	aiScene* Ascene;
	Animation animation;
	Assimp::Importer importer;
	importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_GenBoundingBoxes);
	Ascene = importer.GetOrphanedScene();
	// check for errors
	if (!Ascene || !Ascene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	if (Ascene->mNumAnimations == 0) {
		return;
	}

	for (int i = 0; i < Ascene->mAnimations[0]->mNumChannels; i++) {
		ai_nodes_anim.push_back(Ascene->mAnimations[0]->mChannels[i]);
		std::cout << "GET animation " << Ascene->mAnimations[0]->mChannels[i]->mNodeName.C_Str() << endl;
		animation.anims.push_back(AnimNode(FindAiNode(Ascene->mAnimations[0]->mChannels[i]->mNodeName.data),
			Ascene->mAnimations[0]->mChannels[i]));
	}
	animation.name = name;
	animation.end_time = Ascene->mAnimations[0]->mDuration;
	Anims.push_back(animation);
}

void Model::loadIdleAnimaitons(string path)
{
	aiScene* Ascene;
	Assimp::Importer importer;
	importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_GenBoundingBoxes);
	Ascene = importer.GetOrphanedScene();
	// check for errors
	if (!Ascene || !Ascene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	if (Ascene->mNumAnimations == 0) {
		return;
	}

	for (int i = 0; i < Ascene->mAnimations[0]->mNumChannels; i++) {
		ai_nodes_anim.push_back(Ascene->mAnimations[0]->mChannels[i]);
		std::cout << "GET animation " << Ascene->mAnimations[0]->mChannels[i]->mNodeName.C_Str() << endl;
		Animations.anims.push_back(AnimNode(FindAiNode(Ascene->mAnimations[0]->mChannels[i]->mNodeName.data),
			Ascene->mAnimations[0]->mChannels[i]));
	}
	Animations.end_time = Ascene->mAnimations[0]->mDuration;
	CurrAnimation = &Animations;
}


void Model::PlayAnimation(float delta) {
	if (not keys->at(87) and not keys->at(83)) CurrAnimation.resetCur(&Animations);
		CurrAnimation.Play(delta);
};


Animation* Model::FindAnimation(std::string anim_to_find)
{
	for (int i = 0; i < animations.size(); i++)
	{
		if (animations[i].name == anim_to_find)
			return &animations[i];
	}
	return nullptr;
}

void Model::key_callback(int key, int action)
{
	if(action)
		switch (key)
		{
		case 83:
			CurrAnimation.resetCur(&*find_if(begin(Anims), end(Anims), [](Animation& a) {return a.name == "WalkBackWard";}));
			break;
		case 87:
			CurrAnimation.resetCur(&*find_if(begin(Anims), end(Anims), [](Animation& a) {return a.name == "Walking"; }));
			break;
		default:
			CurrAnimation.resetCur(&Animations);
			break;
		}	
}

Bone* Model::FindBone(std::string name) {
	for (int i = 0; i < bones.size(); i++)
	{
		if (bones[i].name == name)
			return &bones[i];
	}
	return nullptr;
}
aiNode* Model::FindAiNode(std::string name) {
	for (int i = 0; i < ai_nodes.size(); i++)
	{
		if (ai_nodes[i]->mName.data == name)
			return ai_nodes[i];
	}
	return nullptr;
}

aiNodeAnim* Model::FindAiNodeAnim(std::string name)
{
	for (int i = 0; i < ai_nodes_anim.size(); i++)
	{
		if (ai_nodes_anim[i]->mNodeName.data == name)
			return ai_nodes_anim[i];
	}
	//This function finds the animation with the name we pass in, we called it
	//right after calling our recursiveNodeProcess() function, but this function
	//will only really come into play during the next tutorial, where we cover
	//the actual animation portion of skeletal animation.
	return nullptr;
}

int Model::FindBoneIDByName(std::string name)
{
	for (int i = 0; i < bones.size(); i++)
	{
		if (bones[i].name == name)
			return i;
	}
	//This function finds the position of a certain bone within our  vector.
	//This position is equal to the bone's ID, which is vital to determining the
	//rigging of our model within the vertex shader.
	return -1;    //In case we don't find a bone ID, we return -1.
				  //Just to avoid any confusion later on as to whether or not the
				  //ID was found. (It serves the same purpose as returning nullptцr).
}



void Model::loadModel(string path) {
	// read file via ASSIMP
	Assimp::Importer importer;
	importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes);
	scene = importer.GetOrphanedScene();
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));
	// process ASSIMP's root node recursively
	recursiveNodeProcess(scene->mRootNode);
	globalInverseTransform = glm::inverse(AiToGLMMat4(scene->mRootNode->mTransformation));
	processNode(scene->mRootNode, scene, scene->mRootNode->mTransformation);
	AnimNodeProcess();
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		for (int j = 0; j < scene->mMeshes[i]->mNumBones; j++) {
			std::string b_name = scene->mMeshes[i]->mBones[j]->mName.data;
			glm::mat4 b_mat = glm::transpose(AiToGLMMat4(scene->mMeshes[i]->mBones[j]->mOffsetMatrix));
			std::cout << "Bone" << j << " in meshes["<< i <<"] " << scene->mMeshes[i]->mName.C_Str() <<" "<< b_name << std::endl;
		
			Bone bone(&meshes[i],j, b_name, b_mat, FindAiNode(b_name), FindAiNodeAnim(b_name));
			
				//bone.node->mTransformation.Transpose();
				if (bone.animNode == nullptr)
					std::cout << "No Animations were found for " + b_name << std::endl;
				bones.push_back(bone);
		}
	}
	for (int i = 0; i < bones.size(); i++)
	{
		std::string b_name = bones[i].name;
		std::string parent_name = FindAiNode(b_name)->mParent->mName.data;
		Bone* p_bone = FindBone(parent_name);
		bones[i].parent_bone = p_bone;
		if (p_bone == nullptr)
			std::cout << "Parent Bone for " << b_name << " does not exist (is nullptr)" << std::endl;
		else {
			cout << "Create a parent bone" << b_name << " " << p_bone->name << endl;
		}
	}
	skel.Init(bones, globalInverseTransform);
	for (int i = 0; i < meshes.size(); i++) {
		vector<Bone*> bonestep;
		for (int j = 0; j < bones.size(); j++) 
			if (&meshes[i] == bones[j].mesh) bonestep.push_back(&bones[j]);

		meshes[i].sceneLoaderSkeleton.Init(bonestep, globalInverseTransform);
	}
}



void Model::recursiveNodeProcess(aiNode* node) {
	ai_nodes.push_back(node);
	for (int i = 0; i < node->mNumChildren; i++)
		recursiveNodeProcess(node->mChildren[i]);
}
void Model::processNode(aiNode *node, const aiScene *scene, aiMatrix4x4 transformation)
{
	// обработать все полигональные сетки в узле(если есть)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene, transformation));

	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, transformation);
	}
}

void Model::Draw(Shader& shader)
{
	for (auto&& mesh : meshes) {
		mesh.bindTexture(shader);
		mesh.DrawSelf();
	}
}

void Model::Draw()
{
	for (auto&& mesh : meshes) {
		mesh.DrawSelf();
	}
}

void Model::AnimNodeProcess() {
	if (scene->mNumAnimations == 0) {
		return;
	}
	for (int i = 0; i < scene->mAnimations[0]->mNumChannels; i++) {
		ai_nodes_anim.push_back(scene->mAnimations[0]->mChannels[i]);
		std::cout << "GET animation "<< scene->mAnimations[0]->mChannels[i]->mNodeName.C_Str() <<endl;
		Animations.anims.push_back(AnimNode(FindAiNode(scene->mAnimations[0]->mChannels[i]->mNodeName.data),
			scene->mAnimations[0]->mChannels[i]));
	}
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene, aiMatrix4x4 transformation)
{
	glm::vec3 center;
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.Position = { mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z };
		center += vertex.Position;
		vertex.Normal = {mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z};
		if (mesh->mTextureCoords[0]) // сетка обладает набором текстурных координат?
			vertex.TexCoords = { mesh->mTextureCoords[0][i].x , mesh->mTextureCoords[0][i].y};
		else vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		if (mesh->mTangents != nullptr)
		{
			vertex.Tangent = { mesh->mTangents[i].x ,mesh->mTangents[i].y ,mesh->mTangents[i].z };
			vertex.Bitangent = { mesh->mBitangents[i].x ,mesh->mBitangents[i].y ,mesh->mBitangents[i].z };

		}
		else {
			vertex.Tangent = { 0,0,0 };
			vertex.Bitangent = { 0,0,0 };
		}
		vertices.push_back(vertex);
	}
	center /= mesh->mNumVertices;
	
	int WEIGHTS_PER_VERTEX = 4;
	int boneArraysSize = mesh->mNumVertices*WEIGHTS_PER_VERTEX;
	std::vector<float> boneWeights;
	boneWeights.resize(boneArraysSize);
	for (int i = 0; i < mesh->mNumBones; i++) {
		aiBone* aiBone = mesh->mBones[i];
		for (int j = 0; j < aiBone->mNumWeights; j++)
		{
			aiVertexWeight weight = aiBone->mWeights[j];
			unsigned int vertexStart = weight.mVertexId * WEIGHTS_PER_VERTEX;
			for (int k = 0; k < WEIGHTS_PER_VERTEX; k++)
			{
				if (boneWeights.at(vertexStart + k) == 0)
				{
					boneWeights.at(vertexStart + k) = weight.mWeight;
					vertices.at(weight.mVertexId).id[k] = i;
					vertices.at(weight.mVertexId).weight[k] = weight.mWeight;
					break;
				}
			}
		}
	}
	//oбработка индексов
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
		// обработка материала
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	std::vector<Texture> opacityMaps = loadMaterialTextures(material, aiTextureType_OPACITY, "texture_opacity");
	textures.insert(textures.end(), opacityMaps.begin(), opacityMaps.end());
	return Mesh(vertices, indices, textures, center);
}



vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory, type == aiTextureType_NORMALS);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}


unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = SOIL_load_image(filename.c_str(), &width, &height, &nrComponents, !gamma ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	
	if (data)
	{
		
		glBindTexture(GL_TEXTURE_2D, textureID);
		if (!gamma) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		SOIL_free_image_data(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		SOIL_free_image_data(data);
	}

	return textureID;
}