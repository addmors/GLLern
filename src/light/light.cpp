#include "light.h"

light::light(GLfloat* vertices_start, GLfloat* vertices_end,
		GLuint* indices_start, GLuint* indices_end, std::vector<glm::vec3> &pos) {
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
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void light::UseLight(glm::mat4 view, glm::mat4 projection, std::vector<glm::vec3> &lightPos)
{	
	glm::mat4 model = glm::mat4();
	lightShader.Use();
	glBindVertexArray(lightVAO);
	lightShader.SetMat4("view", view);
	lightShader.SetMat4("projection", projection);
	for (GLuint i = 0; i < lightPos.size(); i++) {
		model = glm::mat4();
		model = glm::translate(model, lightPos[i]);
		model = glm::scale(model, glm::vec3(0.2f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
		lightShader.SetMat4("model", model);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}
