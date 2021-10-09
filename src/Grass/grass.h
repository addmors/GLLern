#pragma once
#include "../shader/Shad.h"
#include "../Loader.h"
#include <functional>
#include <random>
#include <vector>
using std::vector;
class Terrian;

struct VertexGrass {
	glm::vec4 Position;
	glm::vec3 Normal;
};

typedef float (Terrian::*FooMethodHeight)(float, float);
typedef glm::vec3 (Terrian::* FooMethodNormal)(float, float);


static float transparentVertices[] = {
	// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
	1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};

class TextureGrass {
public:

	unsigned int averageDistans;
	unsigned char* dataCoverage;
	unsigned VAO, VBO;
	unsigned int transparentTexture;
	unsigned int instanceVBO;
	unsigned int* data;
	float step;
	std::vector<glm::vec3> vegetation;

	TextureGrass(const char* grassPath, Terrian* t, FooMethodHeight getheight, unsigned _averageDistans, float _step = 0.5) :averageDistans(_averageDistans), step(_step){

		vegetation.resize(averageDistans * averageDistans * 4 / step / step);
		unsigned k = 0;
		for (float i = -static_cast<float>(averageDistans); i < static_cast<float>(averageDistans); i += step)

			for (float j = -static_cast<float>(averageDistans); j < static_cast<float>(averageDistans); j += step) {
				vegetation.at(k) = { i, (t->*getheight)(i, j) + 0.3, j };
				k++;
			}
		
		transparentTexture = loadTextureWithAlpha(grassPath);
		
		
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
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(0));
		glVertexAttribDivisor(2, 1);

		glBindVertexArray(0);
	};
	void draw(glm::mat4& projection, glm::mat4& view) {
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, transparentTexture);
		glDisable(GL_CULL_FACE);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, vegetation.size());
		glEnable(GL_CULL_FACE);
		glBindVertexArray(0);
	}
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
	GLuint VAO; // https://www.opengl.org/wiki/Vertex_Specification#Vertex_Array_Object
	GLuint posVbo; // https://www.opengl.org/wiki/Vertex_Specification#Vertex_Buffer_Object
	
	
	Grass(Terrian* t, FooMethodHeight getheight, FooMethodNormal getNormal) {
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
	void draw() {
		glBindVertexArray(VAO);
		glPatchParameteri(GL_PATCH_VERTICES, 1);
		glDrawArrays(GL_PATCHES, 0, numClusters);
		glBindVertexArray(0);
	}
	void genrandomTexture(unsigned int* randTexture) {

		const GLuint randTexSize = 256;
		GLfloat randTexData[randTexSize];
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis(0.0f, 1.0f);
		std::generate(randTexData, randTexData + randTexSize, [&]() {return dis(gen); });
		// Create and tune random texture.
		glGenTextures(1, randTexture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_1D, *randTexture);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage1D(GL_TEXTURE_1D, 0, GL_R16F, randTexSize, 0, GL_RED, GL_FLOAT, randTexData);
		
	}
};

