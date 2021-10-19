#pragma once
#include "../shader/Shad.h"
#include "../Loader.h"
#include "../AABB.h"
#include "../Terrian.h"
#include <vector>
#include <utility>

using std::vector;


typedef float (Terrian::* FooMethodHeight)(float, float);
typedef glm::vec3(Terrian::* FooMethodNormal)(float, float);
typedef float (Terrian::* FooMethodSize)();

struct VertexGrass {
	glm::vec4 Position;
	glm::vec3 Normal;
};

static float transparentVertices[] = {
	// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
	-0.5f,  0.5f,    0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,    0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,    0.0f,  1.0f,  1.0f,

	-0.5f,  0.5f,    0.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,    0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,    0.0f,  1.0f,  0.0f,

	 0.25f,  0.5f, -0.43f,  0.0f,  0.0f,
	 0.25f, -0.5f, -0.43f,  0.0f,  1.0f,
	-0.25f, -0.5f,  0.43f,  1.0f,  1.0f,

	 0.25f,  0.5f, -0.43f,  0.0f,  0.0f,
	-0.25f, -0.5f,  0.43f,  1.0f,  1.0f,
	-0.25f,  0.5f,  0.43f,  1.0f,  0.0f,

	 0.25f,  0.5f,  0.43f,  0.0f,  0.0f,
	 0.25f, -0.5f,  0.43f,  0.0f,  1.0f,
	-0.25f, -0.5f, -0.43f,  1.0f,  1.0f,

	 0.25f,  0.5f,  0.43f,  0.0f,  0.0f,
	-0.25f, -0.5f, -0.43f,  1.0f,  1.0f,
	-0.25f,  0.5f, -0.43f,  1.0f,  0.0f
};

class TextureGrass {
public:

	unsigned int averageDistans;
	float step;
	glm::vec2 pos;
	float maxHeight;
	float minHeight;
	unsigned char* dataCoverage;
	unsigned VAO, VBO;
	unsigned int transparentTexture;
	unsigned int randTexture;
	unsigned int instanceVBO;
	unsigned int* data;
	float sizeofgrass;
	void Delete();
	TextureGrass() = default;
	TextureGrass(const char* grassPath, Terrian* t, 
				FooMethodHeight getheight, FooMethodNormal getNormal, glm::vec2 _pos,  unsigned _averageDistans, float _step = 4);

	void draw(glm::mat4& projection, glm::mat4& view);
	
};

class Cells {
public: 
	Terrian* _t;
	FooMethodHeight _getHeight;
	FooMethodNormal _getNormal;
	float _countInLine;
	float size;
	float sizeCell;
	const char* pathTextures;
	std::vector<TextureGrass> cellGrasses;
	std::vector<AABB> cellAABBs;
	Cells(const char* grassPath, Terrian* t, FooMethodHeight getHeight, FooMethodNormal getNormal, FooMethodSize getSize, float countLine);
	~Cells() {
		AABB::destroy();
	};
};



class Grass
{
public:
	const int numNodes = 100; // Количество узлов решётки вдоль одной стороны.
	const GLfloat gridStep = 2.0f; // Шаг решётки.
	// Максимальные смещения в горизонтальной плоскости:
	const GLfloat xDispAmp = 2.0f;
	const GLfloat zDispAmp = 2.0f;
	const GLfloat yDispAmp = 0.3f; // Максимальное смещение по вертикали.
	int numClusters = numNodes * numNodes; // Количество кустов.
	VertexGrass* vertices = new VertexGrass[numClusters]; // Буфер для генерируемых вершин.
	GLuint VAO, posVbo;
	
	Grass(Terrian* t, FooMethodHeight getheight, FooMethodNormal getNormal);
	void draw();
};

