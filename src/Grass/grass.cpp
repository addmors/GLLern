#include "grass.h"
#include "../Terrian.h"

class Terrian;
typedef float (Terrian::* FooMethodHeight)(float, float);
typedef glm::vec3(Terrian::* FooMethodNormal)(float, float);
typedef float (Terrian::* FooMethodSize)();




Cells::Cells(const char* grassPath, Terrian* t, FooMethodHeight getHeight, FooMethodNormal getNormal, FooMethodSize getSize, float countLine) :
	_t(t),
	_getHeight(getHeight),
	_getNormal(getNormal),
	_countInLine(countLine),
	size((t->*getSize)()),
	sizeCell(size / _countInLine)
{
	AABBcell::InitVAOVBO();
	float count = _countInLine * _countInLine;
	cellGrasses.resize(count);
	cellAABBs.resize(count);
	for (int i = 0; i < _countInLine; i++)
		for (int j = 0; j < _countInLine; j++)
		{
			float posX = (sizeCell - size) / 2 + sizeCell * i;
			float posZ = (sizeCell - size) / 2 + sizeCell * j;
			cellGrasses.at(i + j * _countInLine) = std::move(TextureGrass(grassPath, _t, _getHeight, _getNormal,
				{
					posX,
					posZ //pos
				}, sizeCell / 2));

			auto& tempcellGrasses = cellGrasses.at(i + j * _countInLine);

			cellAABBs.at(i + j * _countInLine) = AABBcell(
				{
					sizeCell,
					tempcellGrasses.maxHeight - tempcellGrasses.minHeight,
					sizeCell
				}, //scale
							{
								posX,
								(tempcellGrasses.maxHeight + tempcellGrasses.minHeight) / 2,
								posZ //pos
							});
		}
}

void TextureGrass::Delete() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &instanceVBO);
}

TextureGrass::TextureGrass(const char* grassPath, Terrian* t, FooMethodHeight getheight,
		FooMethodNormal getNormal, glm::vec2 _pos, unsigned _averageDistans, float _step) :averageDistans(_averageDistans), step(_step), pos(_pos) {
	minHeight = (t->*getheight)(-static_cast<float>(averageDistans), -static_cast<float>(averageDistans));
	maxHeight =minHeight;

	std::vector<glm::vec3> vegetation(averageDistans * averageDistans * 8 / step / step, glm::vec3(0,0,0));
	sizeofgrass = vegetation.size() / 2;
	unsigned k = 0;
	(t->*getheight)(2,2);
	for (float i = -static_cast<float>(averageDistans); i < static_cast<float>(averageDistans); i += step)

		for (float j = -static_cast<float>(averageDistans); j < static_cast<float>(averageDistans); j += step) {
			float height = (t->*getheight)(i + pos.x, j + pos.y);
			vegetation.at(k) = { i+pos.x, height, j+pos.y};
			vegetation.at(k + 1) = (t->*getNormal)(i+pos.x, j+pos.y);
			if (height > maxHeight) maxHeight = height;
			if (height < minHeight) minHeight = height;
			k += 2;
		}

	maxHeight += 1;
	transparentTexture = loadTextureWithAlpha(grassPath);

	genrandomTexture(&randTexture);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));



	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vegetation.size(), &vegetation[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);

	glBindVertexArray(0);
};

void TextureGrass::draw(glm::mat4& projection, glm::mat4& view) {
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, transparentTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, randTexture);
	glDisable(GL_CULL_FACE);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 18, sizeofgrass);
	glEnable(GL_CULL_FACE);
	glBindVertexArray(0);
}



Grass::Grass(Terrian* t, FooMethodHeight getheight, FooMethodNormal getNormal) {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<GLfloat> xDisp(-xDispAmp, xDispAmp);
	std::uniform_real_distribution<GLfloat> yDisp(-yDispAmp, yDispAmp);
	std::uniform_real_distribution<GLfloat> zDisp(-zDispAmp, zDispAmp);
	std::uniform_int_distribution<GLint> numStems(12, 64); // Количество стеблей.
	for (int i = 0; i < numNodes; ++i) {
		for (int j = 0; j < numNodes; ++j) {
			const int idx = (i * numNodes + j);
			vertices[idx].Position.x = (i - numNodes / 2) * gridStep + xDisp(mt);
			vertices[idx].Position.z = (j - numNodes / 2) * gridStep + zDisp(mt);
			vertices[idx].Position.y = (t->*getheight)(vertices[idx].Position.x, vertices[idx].Position.z);
			vertices[idx].Position.w = numStems(mt);
			vertices[idx].Normal = (t->*getNormal)(vertices[idx].Position.x, vertices[idx].Position.z);
		}
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &posVbo);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);

	glBufferData(GL_ARRAY_BUFFER, numClusters * sizeof(VertexGrass), &vertices[0], GL_DYNAMIC_DRAW);
	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexGrass), (void*)offsetof(VertexGrass, Position));
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexGrass), (void*)offsetof(VertexGrass, Normal));

	glBindVertexArray(0);

	//glFinish();
	delete[] vertices;
}
void Grass::draw() {
	glBindVertexArray(VAO);
	glPatchParameteri(GL_PATCH_VERTICES, 1);
	glDrawArrays(GL_PATCHES, 0, numClusters);
	glBindVertexArray(0);
}