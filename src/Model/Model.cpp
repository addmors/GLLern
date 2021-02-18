#include "Model.h"

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);


void Model::DrawAnim(Shader shader, glm::mat4 model, glm::mat4 wiew, glm::mat4 projection) {
	if (!anim)    //If the object is rigged...
	{
		//UpdateSkeleton();    //... we update the skeleton (remember that this

		Animations.PlayAnimation(1);
		for (int i = 0; i <meshes.size(); i++) {
			if (meshes[i].sceneLoaderSkeleton.bones.size()>0) {
				meshes[i].sceneLoaderSkeleton.Update();
				meshes[i].DrawRigged(shader, model, wiew, projection);
			}
			else meshes[i].Draw(shader);
		}
	}
	else
	{

		for (int i = 0; i < meshes.size(); i++)
			meshes[i].Draw(shader);
	}
}
void Model::Draw(Shader shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}



Animation* Model::FindAnimation(std::string anim_to_find)
{
	for (int i = 0; i < animations.size(); i++)
	{
		if (animations[i].name == anim_to_find)
			return &animations[i];
	}
	return nullptr;
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
				  //ID was found. (It serves the same purpose as returning nullptr).
}



void Model::loadModel(string path) {
	// read file via ASSIMP
	Assimp::Importer importer;

	importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_GenBoundingBoxes);
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
	aiNode* m_rootNode = scene->mRootNode;
	recursiveNodeProcess(m_rootNode);
	globalInverseTransform = glm::inverse(AiToGLMMat4(scene->mRootNode->mTransformation));
	aiMatrix4x4 transform = scene->mRootNode->mTransformation;
	processNode(scene->mRootNode, scene, transform);
	AnimNodeProcess();
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		for (int j = 0; j < scene->mMeshes[i]->mNumBones; j++) {
			std::string b_name = scene->mMeshes[i]->mBones[j]->mName.data;
			glm::mat4 b_mat = glm::transpose(AiToGLMMat4(scene->mMeshes[i]->mBones[j]->mOffsetMatrix));
			std::cout << "Bone" << j << " in meshes["<< i <<"] " << scene->mMeshes[i]->mName.C_Str() <<" "<< b_name << std::endl;
		
			Bone bone(&meshes[i],j, b_name, b_mat);
				bone.node = FindAiNode(b_name);
				bone.animNode = FindAiNodeAnim(b_name);
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
	//if (meshes.size() > 0) {//If there are, in fact, meshes.
	//skel.Init(bones, globalInverseTransform);
	//}
	//skel.PlayAnimation(Anim_Test_Idle2, true,true);
	for (int i = 0; i < meshes.size(); i++) {
		vector<Bone> bonestep;
		for (int j = 0; j < bones.size(); j++) {
			if (&meshes[i] == bones[j].mesh) {
				bonestep.push_back(bones[j]);
			}
			
		}
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
	// ���������� ��� ������������� ����� � ����(���� ����)
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
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{

		Vertex vertex;
		glm::vec4 vector(1.0f);

			
		aiVector3D v =  mesh->mVertices[i];
		vector.x =v.x;
		vector.y = v.y;
		vector.z = v.z;
		vertex.Position =  vector;
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0]) // ����� �������� ������� ���������� ���������?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		//vector.x = mesh->mTangents[i].x;
		//vector.y = mesh->mTangents[i].y;
		//vector.z = mesh->mTangents[i].z;
		//vertex.Tangent = vector;
		//// bitangent
		//vector.x = mesh->mBitangents[i].x;
		//vector.y = mesh->mBitangents[i].y;
		//vector.z = mesh->mBitangents[i].z;
		//vertex.Bitangent = vector;
		vertices.push_back(vertex);
	}
	int WEIGHTS_PER_VERTEX = 4;
	int boneArraysSize = mesh->mNumVertices*WEIGHTS_PER_VERTEX;
	std::vector<int> boneIDs;
	boneIDs.resize(boneArraysSize);
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
					boneIDs.at(vertexStart + k) = i;
					vertices.at(weight.mVertexId).id[k] = i;
					vertices.at(weight.mVertexId).weight[k] = weight.mWeight;
					break;
				}
			}
		}
	}
	//o�������� ��������
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
		// ��������� ���������
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());


	return Mesh(vertices, indices, textures);
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
			texture.id = TextureFromFile(str.C_Str(), this->directory);
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

	unsigned char* data = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

	if (data)
	{
		/*GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;*/

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
	