#include "Terrian.h"

float Terrian::getHeightOfTerrian(float x, float y) {

	x += SIZE / 2;
	y += SIZE / 2;
	float terrianInY = y / scaleY+0.5;
	float terrianInX = x / scaleX+0.5;

	int gridX = floor(terrianInX)-1;
	int gridY = floor(terrianInY)-1;

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
};