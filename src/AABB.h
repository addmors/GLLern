#pragma once
#include "shader\Shad.h"


static GLfloat RectVertices[] = {
	-1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	// front face
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,
	// left face
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	// right face
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 // bottom face
	 -1.0f, -1.0f, -1.0f,
	  1.0f, -1.0f, -1.0f,
	  1.0f, -1.0f,  1.0f,
	  1.0f, -1.0f,  1.0f,
	 -1.0f, -1.0f,  1.0f,
	 -1.0f, -1.0f, -1.0f,
	 // top face
	 -1.0f,  1.0f, -1.0f,
	  1.0f,  1.0f , 1.0f,
	  1.0f,  1.0f, -1.0f,
	  1.0f,  1.0f,  1.0f,
	 -1.0f,  1.0f, -1.0f,
	 -1.0f,  1.0f,  1.0f
};

class AABBcell
{
private:
    glm::vec3 _scale, _pos;

public: 

	static unsigned cubeVAO;
	static unsigned cubeVBO;

	static void InitVAOVBO() {
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(RectVertices), RectVertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	AABBcell(glm::vec3 scale, glm::vec3 pos) : _scale(scale), _pos(pos){};
	AABBcell() = default;
	static void draw() {
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	} 
	static void destroy() {
		glDeleteVertexArrays(1, &cubeVAO);
		glDeleteBuffers(1, &cubeVBO);
	};
	glm::vec3 getPos() { return _pos; };
	glm::vec3 getScale() { return _scale; };
};

