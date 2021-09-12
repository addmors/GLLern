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
	vector<unsigned int> indices;
	std::shared_ptr<btHeightfieldTerrainShape> heightfieldShape;
	unsigned VAO, VBO, EBO;
	vector<float> height_map;

	unsigned int blendMap, rTexture, gTexture, bTexture, backGround;
	int VERTEX_COUNT_X, VERTEX_COUNT_Y;

public:
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

	Terrian(float sizeTerrian, const char* pathHeightMap) :SIZE(sizeTerrian) {

		int nrComponents;
		///gen buffer in image 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		data = stbi_load(pathHeightMap, &VERTEX_COUNT_X, &VERTEX_COUNT_Y, &nrComponents, 0);
		this->scaleX = SIZE / VERTEX_COUNT_X;
		this->scaleY = SIZE / VERTEX_COUNT_Y;
		vertexs.resize(VERTEX_COUNT_X * VERTEX_COUNT_X);
		height_map.resize(VERTEX_COUNT_X * VERTEX_COUNT_X);
		
		///gen VErtex map 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		for (int i = 0; i < VERTEX_COUNT_X; i++)
			for (int j = 0; j < VERTEX_COUNT_Y; j++) {
				float z = getHeightinMinMaxinter(Height(data, i, j));
				height_map[j * VERTEX_COUNT_X + i] = z;
				
				vertexs[j * VERTEX_COUNT_X + i] = {
						glm::vec3((float)i / ((float)VERTEX_COUNT_X - 1) * VERTEX_COUNT_X*scaleX, z , (float)j / ((float)VERTEX_COUNT_Y - 1) * VERTEX_COUNT_Y*scaleY),
						getNormal(data,i,j),
						glm::vec2((float)i / ((float)VERTEX_COUNT_X - 1),(float)j / ((float)VERTEX_COUNT_Y - 1))
				};

			}

		///gen BtFileld
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		heightfieldShape = std::make_shared<btHeightfieldTerrainShape>(
			VERTEX_COUNT_X,
			VERTEX_COUNT_Y,
			&height_map[0],
			0,
			MN_HEIGHT, MX_HEIGHT,
			1, PHY_FLOAT, false
		);
		btVector3 scale{ scaleX* VERTEX_COUNT_X/(VERTEX_COUNT_X-1),1,scaleY* VERTEX_COUNT_Y / (VERTEX_COUNT_Y-1)};
		heightfieldShape->setLocalScaling(scale);
		heightfieldShape->setUseDiamondSubdivision(true);

		
		///gen Indices 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		int pointer = 0;
		indices.resize((VERTEX_COUNT_X - 1) * (VERTEX_COUNT_Y - 1) * 6);
		for (int gz = 0; gz < VERTEX_COUNT_Y - 1; gz++) {
			for (int gx = 0; gx < VERTEX_COUNT_X - 1; gx++) {
				int topLeft = (gz * VERTEX_COUNT_X) + gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz + 1) * VERTEX_COUNT_X) + gx;
				int bottomRight = bottomLeft + 1;
				if ((gz + gx) % 2 == 0) {
					indices[(gz * (VERTEX_COUNT_X - 1) + gx) * 6] = (bottomRight);
					indices[(gz * (VERTEX_COUNT_X - 1) + gx) * 6 + 1] = (topRight);
					indices[(gz * (VERTEX_COUNT_X - 1) + gx) * 6 + 2] = (topLeft);
					indices[(gz * (VERTEX_COUNT_X - 1) + gx) * 6 + 3] = (bottomLeft);
					indices[(gz * (VERTEX_COUNT_X - 1) + gx) * 6 + 4] = (bottomRight);
					indices[(gz * (VERTEX_COUNT_X - 1) + gx) * 6 + 5] = (topLeft);
				}
				else {
					indices[(gz * (VERTEX_COUNT_X - 1) + gx) * 6] = (topLeft);
					indices[(gz * (VERTEX_COUNT_X - 1) + gx) * 6 + 1] = (bottomLeft);
					indices[(gz * (VERTEX_COUNT_X - 1) + gx) * 6 + 2] = (topRight);
					indices[(gz * (VERTEX_COUNT_X - 1) + gx) * 6 + 3] = (topRight);
					indices[(gz * (VERTEX_COUNT_X - 1) + gx) * 6 + 4] = (bottomLeft);
					indices[(gz * (VERTEX_COUNT_X - 1) + gx) * 6 + 5] = (bottomRight);
				}
			}
		}
		setupTerrarian();
	}
	void setupTerrarian()
	{

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertexs.size() * sizeof(vertexs), &vertexs[0], GL_STATIC_DRAW);

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
		return pHeightMap[(Y + (X * MAP_SIZE)) * 4];
	}

	float getHeightinMinMaxinter(float x) {
		return ((MX_HEIGHT - MN_HEIGHT) * (x) / 256.0F) + MN_HEIGHT;
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

	float getHeightOfTerrian(float x, float y);
};
