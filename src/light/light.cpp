#include "light.h"

light::light(GLfloat* vertices_start, GLfloat* vertices_end,
		GLuint* indices_start, GLuint* indices_end) {
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (vertices_end - vertices_start) * sizeof(vertices_start) / 2, vertices_start, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (vertices_end - vertices_start) * sizeof(vertices_start) / 2, indices_start, GL_STATIC_DRAW);
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}


void light::Draw()
{	
	glBindVertexArray(lightVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}
//
//glDisable(GL_CULL_FACE);
//glEnable(GL_DEPTH_TEST);
//glm::mat4 model = glm::mat4();
//lightShader.Use();
//lightShader.SetVec3("color", color_);
//
//for (GLuint i = 0; i < lightPos.size(); i++) {
//	glBindVertexArray(lightVAO);
//	model = glm::mat4();
//	model = glm::translate(model, lightPos[i]);
//	model = glm::scale(model, glm::vec3(0.2f));
//	//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
//
//}
//glDisable(GL_DEPTH_TEST);
//glEnable(GL_CULL_FACE);
