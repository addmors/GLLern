#pragma once
#include "../shader/Shad.h"
#include <random>

class Grass
{
public:
	const int numNodes = 14; // Количество узлов решётки вдоль одной стороны.
	const GLfloat gridStep = 3.0f; // Шаг решётки.
	// Максимальные смещения в горизонтальной плоскости:
	const GLfloat xDispAmp = 5.0f;
	const GLfloat zDispAmp = 5.0f;
	const GLfloat yDispAmp = 0.3f; // Максимальное смещение по вертикали.
	int numClusters = numNodes * numNodes; // Количество кустов.
	GLfloat* vertices = new GLfloat[numClusters * 4]; // Буфер для генерируемых вершин.
	GLuint vao; // https://www.opengl.org/wiki/Vertex_Specification#Vertex_Array_Object
	GLuint posVbo; // https://www.opengl.org/wiki/Vertex_Specification#Vertex_Buffer_Object
	Grass() {
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<GLfloat> xDisp(-xDispAmp, xDispAmp);
		std::uniform_real_distribution<GLfloat> yDisp(-yDispAmp, yDispAmp);
		std::uniform_real_distribution<GLfloat> zDisp(-zDispAmp, zDispAmp);
		std::uniform_int_distribution<GLint> numStems(12, 64); // Количество стеблей.
		for (int i = 0; i < numNodes; ++i) {
			for (int j = 0; j < numNodes; ++j) {
				const int idx = (i * numNodes + j) * 4;
				vertices[idx] = (i - numNodes / 2) * gridStep + xDisp(mt);
				vertices[idx + 1] = yDisp(mt);
				vertices[idx + 2] = (j - numNodes / 2) * gridStep + zDisp(mt);
				vertices[idx + 3] = numStems(mt);
			}
		}
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &posVbo);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, posVbo);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numClusters * 4, vertices, GL_STATIC_DRAW);
		glFinish();
		delete[] vertices;
	}
	void draw() {
		glBindVertexArray(vao);
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

