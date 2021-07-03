#pragma once
#include "shader/Shad.h";
#include <string>
#include <utility>
#include <stb_image.h>
#include<memory>
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#define MAP_SIZE 256
#define MX_HEIGHT 10
#define MN_HEIGHT -10
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

static 

class Terrian {
private:
	float scaleX;
	float scaleY;
	float MAX_HIEGHT;

	float  SIZE;
	vector<VertexTerrarian> vertexs;
	vector<unsigned int> indices;
	std::shared_ptr<btHeightfieldTerrainShape> heightfieldShape;
	unsigned VAO, VBO, EBO;
	vector<float> height_map;

public:
	float getSize() { return SIZE; };
	std::pair<float, float> getScaleTerrian() { return std::make_pair(scaleX, scaleY); };
	std::shared_ptr<btHeightfieldTerrainShape> getHeightField() { return heightfieldShape; };

	Terrian(float sizeTerrian, const char* pathHeightMap) :SIZE(sizeTerrian) {

		int VERTEX_COUNT_X, VERTEX_COUNT_Y, nrComponents;
		///gen buffer in image 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		unsigned char* data = stbi_load(pathHeightMap, &VERTEX_COUNT_X, &VERTEX_COUNT_Y, &nrComponents, 0);
		this->scaleX = VERTEX_COUNT_X / SIZE;
		this->scaleY = VERTEX_COUNT_Y / SIZE;
		vertexs.resize(VERTEX_COUNT_X * VERTEX_COUNT_X);
		height_map.resize(VERTEX_COUNT_X * VERTEX_COUNT_X);
		float z = getHeightinMinMaxinter(Height(data, 0, 0));
		float minHeight = z;
		float maxHeight = z;

		///gen VErtex map 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		for (int i = 0; i < VERTEX_COUNT_X; i++)
			for (int j = 0; j < VERTEX_COUNT_Y; j++) {
				float z = getHeightinMinMaxinter(Height(data, i, j));
				height_map[j * VERTEX_COUNT_X + i] = z;
				if (z < minHeight) {
					minHeight = z;
				}
				if (z > maxHeight) {
					maxHeight = z;
				}
				vertexs[j * VERTEX_COUNT_X + i] = {
						glm::vec3((float)i / ((float)VERTEX_COUNT_X - 1) * SIZE, z , (float)j / ((float)VERTEX_COUNT_Y - 1) * SIZE),
						glm::vec3(0,1,0),
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
			minHeight, maxHeight,
			1, PHY_FLOAT, false
		);
		heightfieldShape->setLocalScaling({ scaleX,1,scaleY });
		
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
				indices[(gz * (VERTEX_COUNT_X - 1) + gx) * 6] = (topLeft);
				indices[(gz * (VERTEX_COUNT_X - 1) + gx) * 6 + 1] = (bottomLeft);
				indices[(gz * (VERTEX_COUNT_X - 1) + gx) * 6 + 2] = (topRight);
				indices[(gz * (VERTEX_COUNT_X - 1) + gx) * 6 + 3] = (topRight);
				indices[(gz * (VERTEX_COUNT_X - 1) + gx) * 6 + 4] = (bottomLeft);
				indices[(gz * (VERTEX_COUNT_X - 1) + gx) * 6 + 5] = (bottomRight);
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
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}

	unsigned char Height(unsigned char* pHeightMap, int X, int Y) {
		//if (!pHeightMap) return 0;
		// All right - returning our height
		return pHeightMap[(Y + (X * MAP_SIZE)) * 3];
	}

	float getHeightinMinMaxinter(float x) {
		return ((MX_HEIGHT - MN_HEIGHT) * (x) / 256.0F) + MN_HEIGHT;
	}
};
