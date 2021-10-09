#include "Terrian.h"


Terrian::Terrian(float sizeTerrian, const char* pathHeightMap) :SIZE(sizeTerrian) {

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
					glm::vec3((float)i / ((float)VERTEX_COUNT_X - 1) * VERTEX_COUNT_X * scaleX, z , (float)j / ((float)VERTEX_COUNT_Y - 1) * VERTEX_COUNT_Y * scaleY),
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
	btVector3 scale{ scaleX * VERTEX_COUNT_X / (VERTEX_COUNT_X - 1),1,scaleY * VERTEX_COUNT_Y / (VERTEX_COUNT_Y - 1) };
	heightfieldShape->setLocalScaling(scale);
	heightfieldShape->setUseDiamondSubdivision(true);


	///gen Indices 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	int pointer = 0;
	indices.resize((VERTEX_COUNT_X - 1) * (VERTEX_COUNT_Y - 1) * 6);
	for (int gz = 0; gz < VERTEX_COUNT_Y - 1; gz++) {
		for (int gx = 0; gx < VERTEX_COUNT_X - 1; gx++) {
			unsigned  topLeft = (gz * VERTEX_COUNT_X) + gx;
			unsigned topRight = topLeft + 1;
			unsigned bottomLeft = ((gz + 1) * VERTEX_COUNT_X) + gx;
			unsigned bottomRight = bottomLeft + 1;
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
	std::cout << indices[390149];
	std::cout << indices[390148];
	std::cout << indices[390147];
	std::cout << indices[390146];



	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertexs.size() * sizeof(VertexTerrarian), &vertexs[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned),
		&indices[0], GL_STATIC_DRAW);
	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTerrarian), (void*)offsetof(VertexTerrarian, Position));
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTerrarian), (void*)offsetof(VertexTerrarian, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTerrarian), (void*)offsetof(VertexTerrarian, TexCoords));
	glBindVertexArray(0);

}

float Terrian::getHeightOfTerrian(float x, float y) {

	x += SIZE / 2;
	y += SIZE / 2;
	float terrianInY = (y / scaleY)+0.5;
	float terrianInX = (x / scaleX)+0.5;

	int gridX = floor(terrianInX)-1;
	int gridY = floor(terrianInY)-1;
	if (gridX < 0) gridX = 0;
	if (gridY < 0) gridY = 0;
	if (gridX > 254) gridX = 254;
	if (gridY > 254) gridY = 254;


	float xCoord = fmod(terrianInX, 1);
	float yCoord = fmod(terrianInY, 1);
	
	if ((gridX + gridY) % 2 == 0) {
		if (xCoord <= yCoord) {
			return barryCentric(
				glm::vec3(0, height_map[gridX + (gridY * VERTEX_COUNT_X)], 0),
				glm::vec3(1, height_map[gridX + ((gridY + 1) * VERTEX_COUNT_X) + 1], 1),
				glm::vec3(0, height_map[gridX + ((gridY + 1) * VERTEX_COUNT_X)], 1),
				glm::vec2(xCoord, yCoord));
		
		}
		else {
			return barryCentric(
				glm::vec3(0, height_map[gridX + ((gridY)*VERTEX_COUNT_X)], 0),
				glm::vec3(1, height_map[gridX + ((gridY + 1) * VERTEX_COUNT_X + 1)], 1),
				glm::vec3(1, height_map[gridX + ((gridY)*VERTEX_COUNT_X) + 1], 0),
				glm::vec2(xCoord, yCoord));

		}
	}
	else {
		if (xCoord <= (1 - yCoord)) {
			return barryCentric(
				glm::vec3(0, height_map[gridX + (gridY * VERTEX_COUNT_X)], 0),
				glm::vec3(1, height_map[gridX + 1 + ((gridY)*VERTEX_COUNT_X)], 0),
				glm::vec3(0, height_map[gridX + ((gridY + 1) * VERTEX_COUNT_X)], 1),
				glm::vec2(xCoord, yCoord));
		}
		else {
			return barryCentric(
				glm::vec3(1, height_map[gridX + 1 + ((gridY)*VERTEX_COUNT_X)], 0),
				glm::vec3(1, height_map[gridX + ((gridY + 1) * VERTEX_COUNT_X) + 1], 1),
				glm::vec3(0, height_map[gridX + ((gridY + 1) * VERTEX_COUNT_X)], 1),
				glm::vec2(xCoord, yCoord));
		}

		return 0;
	}
}
glm::vec3 Terrian::getNormlofTerrian(float x, float y)
{
	x += SIZE / 2;
	y += SIZE / 2;
	float terrianInY = y / scaleY + 0.5;
	float terrianInX = x / scaleX + 0.5;
	int gridX = floor(terrianInX) - 1;
	int gridY = floor(terrianInY) - 1;
	return getNormal(data,gridX, gridY);
};