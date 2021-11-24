#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include "../shader/Shad.h"
#include "../Skeleton/Skeleton.h"

using namespace std;

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	//tangent
	glm::vec3 Tangent;
	//// bitangent
	glm::vec3 Bitangent;
	//Bone
	glm::vec4 weight;
	glm::ivec4 id;
};
struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh {
public:
	/*  Mesh Data  */
	vector<Vertex> _vertices;
	vector<unsigned int> _indices;
	vector<Texture> _textures;
	Skeleton sceneLoaderSkeleton;
	/*  Functions  */
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, glm::vec3 center);

	void Delete() {
		for (auto texture : _textures) {
			if (texture.id != 0)
				glDeleteTextures(1, &texture.id);
		};
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		};

	void DrawSelf();
	void bindTexture(Shader& shader);
	void SetRigged(Shader& shader);
	void UpdateBoneMat();

	unsigned int VAO, VBO, EBO, VBO_Pos, VBO_UVs, VBO_Nor, VBO_IDs, VBO_Weights;
private:
	/*  Render data  */
	glm::vec3 _center;
	/*  Functions    */
	void setupMesh();
};

