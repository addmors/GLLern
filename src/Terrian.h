#pragma once
#include "shader/Shad.h"
#include "Loader.h"
#include <string>
#include <utility>
#include <stb_image.h>
#include<memory>
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

#define MAP_SIZE 256
#define MX_HEIGHT 40
#define MN_HEIGHT -40
using std::vector;

struct VertexTerrarian {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	//tangent
};

static float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) {
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
};



class Terrian {
private:
	float scaleX;
	float scaleY;
	float MAX_HIEGHT;
	unsigned char* data;

	float  SIZE;
	vector<VertexTerrarian> vertexs;
	vector<unsigned> indices;
	std::shared_ptr<btHeightfieldTerrainShape> heightfieldShape;
	unsigned VAO, VBO, EBO;
	vector<float> height_map;

	unsigned int blendMap, rTexture, gTexture, bTexture, backGround;
	int VERTEX_COUNT_X, VERTEX_COUNT_Y;
public:
	~Terrian() { 
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		stbi_image_free(data); 
	};
	void loadTextures(const char* blendMap, const char* rTexture, const char* gTexture, const char* bTexture, const char* backGround) {
		this->blendMap = loadTextureRGB(blendMap);
		this->rTexture = loadTexture(rTexture);
		this->gTexture = loadTexture(gTexture);
		this->bTexture = loadTexture(bTexture);
		this->backGround = loadTexture(backGround);

	}
	void ActiveTexture() {

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, blendMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, backGround);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, rTexture);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gTexture);

		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, bTexture);

	}
	float getSize() { return SIZE; };
	std::pair<float, float> getScaleTerrian() { return std::make_pair(scaleX, scaleY); };
	std::shared_ptr<btHeightfieldTerrainShape> getHeightField() { return heightfieldShape; };
	
	void setupTerrarian()
	{

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertexs.size() * sizeof(vertexs), &vertexs[0], GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
			&indices[0], GL_STATIC_DRAW);
		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTerrarian), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTerrarian), (void*)offsetof(VertexTerrarian, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTerrarian), (void*)offsetof(VertexTerrarian, TexCoords));
		glBindVertexArray(0);

	}

	Terrian(float sizeTerrian, const char* pathHeightMap);
	void Draw()
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}

	unsigned char Height(unsigned char* pHeightMap, int X, int Y) {
		//if (!pHeightMap) return 0;
		// All right - returning our height
		return pHeightMap[(Y + (X * 256)) * 4];
	}

	float getHeightinMinMaxinter(float x) {
		return ((80) * (x) / 256.0F) + MN_HEIGHT;
	}

	glm::vec3 getNormal(unsigned char* pHeightMap, int x, int z) {
		if (x >= MAP_SIZE) return glm::vec3(0, 1, 0);
		if (z >= MAP_SIZE) return glm::vec3(0, 1, 0);
		if (x <= 0) return glm::vec3(0, 1, 0);
		if (z <= 0) return glm::vec3(0, 1, 0);

		float heightL = getHeightinMinMaxinter(Height(pHeightMap, x - 1, z));
		float heightR = getHeightinMinMaxinter(Height(pHeightMap, x + 1, z));
		float heightD = getHeightinMinMaxinter(Height(pHeightMap, x, z - 1));
		float heightU = getHeightinMinMaxinter(Height(pHeightMap, x, z + 1));
		return glm::normalize(glm::vec3(heightL - heightR, 2, heightD - heightU));
	};

	float getHeightOfTerrian(float, float);
	glm::vec3 getNormlofTerrian(float, float);
	glm::vec3 getNormlofTerrian(unsigned char* , float, float);

	glm::vec3 getTangentofTerrian(float, float);
};
