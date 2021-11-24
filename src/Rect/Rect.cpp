#include "Rect.h"
#include "../Precompile.h"
void Rect::Init()
{
	  vertices = {
		// back face
		-1.0f, -1.0f, -1.0f,   0.0f, 0.0f,     0.0f,  0.0f, -1.0f,     -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, // bottom-left
		 1.0f,  1.0f, -1.0f,   1.0f, 1.0f,     0.0f,  0.0f, -1.0f,     -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, // top-right
		 1.0f, -1.0f, -1.0f,   1.0f, 0.0f,     0.0f,  0.0f, -1.0f,     -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, // bottom-right         
		 1.0f,  1.0f, -1.0f,   1.0f, 1.0f,     0.0f,  0.0f, -1.0f,     -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, // top-right
		-1.0f, -1.0f, -1.0f,   0.0f, 0.0f,     0.0f,  0.0f, -1.0f,     -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, // bottom-left
		-1.0f,  1.0f, -1.0f,   0.0f, 1.0f,     0.0f,  0.0f, -1.0f,     -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, // top-left
		// front face							    
		-1.0f, -1.0f,  1.0f,    0.0f, 0.0f,    0.0f,  0.0f,  1.0f,     1.0, 0.0, 0.0,  0.0,  1.0, 0.0, // bottom-left
		 1.0f, -1.0f,  1.0f,    1.0f, 0.0f,    0.0f,  0.0f,  1.0f,     1.0, 0.0, 0.0,  0.0,  1.0, 0.0, // bottom-right
		 1.0f,  1.0f,  1.0f,    1.0f, 1.0f,    0.0f,  0.0f,  1.0f,     1.0, 0.0, 0.0,  0.0,  1.0, 0.0, // top-right
		 1.0f,  1.0f,  1.0f,    1.0f, 1.0f,    0.0f,  0.0f,  1.0f,     1.0, 0.0, 0.0,  0.0,  1.0, 0.0, // top-right
		-1.0f,  1.0f,  1.0f,    0.0f, 1.0f,    0.0f,  0.0f,  1.0f,     1.0, 0.0, 0.0,  0.0,  1.0, 0.0, // top-left
		-1.0f, -1.0f,  1.0f,    0.0f, 0.0f,    0.0f,  0.0f,  1.0f,     1.0, 0.0, 0.0,  0.0,  1.0, 0.0, // bottom-left
		// left face		  	 			  	    
		-1.0f,  1.0f,  1.0f,    1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,     0.0,-1.0, 0.0,  0.0, 0.0, -1.0,// top-right
		-1.0f,  1.0f, -1.0f,    1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,     0.0,-1.0, 0.0,  0.0, 0.0, -1.0,// top-left
		-1.0f, -1.0f, -1.0f,    0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,     0.0,-1.0, 0.0,  0.0, 0.0, -1.0,// bottom-left
		-1.0f, -1.0f, -1.0f,    0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,     0.0,-1.0, 0.0,  0.0, 0.0, -1.0,// bottom-left
		-1.0f, -1.0f,  1.0f,    0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,     0.0,-1.0, 0.0,  0.0, 0.0, -1.0,// bottom-right
		-1.0f,  1.0f,  1.0f,    1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,     0.0,-1.0, 0.0,  0.0, 0.0, -1.0,// top-right
		// right face		  	 			  	    
		 1.0f,  1.0f,  1.0f,    1.0f, 0.0f,    1.0f,  0.0f,  0.0f,     0.0, 1.0, 0.0,  0.0, 0.0, 1.0,  // top-left
		 1.0f, -1.0f, -1.0f,    0.0f, 1.0f,    1.0f,  0.0f,  0.0f,     0.0, 1.0, 0.0,  0.0, 0.0, 1.0,  // bottom-right
		 1.0f,  1.0f, -1.0f,    1.0f, 1.0f,    1.0f,  0.0f,  0.0f,     0.0, 1.0, 0.0,  0.0, 0.0, 1.0,  // top-right         
		 1.0f, -1.0f, -1.0f,    0.0f, 1.0f,    1.0f,  0.0f,  0.0f,     0.0, 1.0, 0.0,  0.0, 0.0, 1.0,  // bottom-right
		 1.0f,  1.0f,  1.0f,    1.0f, 0.0f,    1.0f,  0.0f,  0.0f,     0.0, 1.0, 0.0,  0.0, 0.0, 1.0,  // top-left
		 1.0f, -1.0f,  1.0f,    0.0f, 0.0f,    1.0f,  0.0f,  0.0f,     0.0, 1.0, 0.0,  0.0, 0.0, 1.0,  // bottom-left     
		// bottom face		  	 			  	    
		-1.0f, -1.0f, -1.0f,    0.0f, 1.0f,    0.0f, -1.0f,  0.0f,     0.0, 0.0, -1.0,-1.0, 0.0, 0.0,// top-right
		 1.0f, -1.0f, -1.0f,    1.0f, 1.0f,    0.0f, -1.0f,  0.0f,     0.0, 0.0, -1.0,-1.0, 0.0, 0.0,// top-left
		 1.0f, -1.0f,  1.0f,    1.0f, 0.0f,    0.0f, -1.0f,  0.0f,     0.0, 0.0, -1.0,-1.0, 0.0, 0.0,// bottom-left
		 1.0f, -1.0f,  1.0f,    1.0f, 0.0f,    0.0f, -1.0f,  0.0f,     0.0, 0.0, -1.0,-1.0, 0.0, 0.0,// bottom-left
		-1.0f, -1.0f,  1.0f,    0.0f, 0.0f,    0.0f, -1.0f,  0.0f,     0.0, 0.0, -1.0,-1.0, 0.0, 0.0,// bottom-right
		-1.0f, -1.0f, -1.0f,    0.0f, 1.0f,    0.0f, -1.0f,  0.0f,     0.0, 0.0, -1.0,-1.0, 0.0, 0.0,// top-right
		// top face			    		     
		-1.0f,  1.0f, -1.0f,    0.0f, 1.0f,    0.0f,  1.0f,  0.0f,     0.0, 0.0, 1.0,  1.0, 0.0, 0.0,// top-left
		 1.0f,  1.0f , 1.0f,    1.0f, 0.0f,    0.0f,  1.0f,  0.0f,     0.0, 0.0, 1.0,  1.0, 0.0, 0.0,// bottom-right
		 1.0f,  1.0f, -1.0f,    1.0f, 1.0f,    0.0f,  1.0f,  0.0f,     0.0, 0.0, 1.0,  1.0, 0.0, 0.0,// top-right     
		 1.0f,  1.0f,  1.0f,    1.0f, 0.0f,    0.0f,  1.0f,  0.0f,     0.0, 0.0, 1.0,  1.0, 0.0, 0.0,// bottom-right
		-1.0f,  1.0f, -1.0f,    0.0f, 1.0f,    0.0f,  1.0f,  0.0f,     0.0, 0.0, 1.0,  1.0, 0.0, 0.0,// top-left
		-1.0f,  1.0f,  1.0f,    0.0f, 0.0f,    0.0f,  1.0f,  0.0f,     0.0, 0.0, 1.0,  1.0, 0.0, 0.0// bottom-left        
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// fill buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()
		* sizeof(float), &vertices.at(0), GL_STATIC_DRAW);
	// link vertex attributes
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(5 * sizeof(float)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	prepareInstanse();
}

void Rect::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void Rect::drawInstance()
{
	if (Instanse.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, bufferInstance);

		glBufferData(GL_ARRAY_BUFFER, Instanse.size() * sizeof(glm::mat4), &Instanse[0], GL_STATIC_DRAW);

		glBindVertexArray(VAO);
		glDrawArraysInstanced(
			GL_TRIANGLES, 0, 36, Instanse.size());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void Rect::buildVerticesSmooth()
{
	return;
}
void Rect::buildVerticesFlat()
{
	return;
};
