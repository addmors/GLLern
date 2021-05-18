#pragma once
#include "../shader/Shad.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class light
{
public:
	Shader lightShader{ "src/shader/shader.vs", "src/shader/lampshad.fs" };
	void UseLight(glm::mat4 view, glm::mat4 projection, std::vector<glm::vec3> &lightPos);
	void Delete() { (glDeleteVertexArrays(1, &lightVAO)); 
					glDeleteBuffers(1, &VBO);
					glDeleteBuffers(1, &EBO);
					}
	light(GLfloat* vertices_start, GLfloat* vertices_end,
		GLuint* indices_start, GLuint* indices_end, std::vector<glm::vec3> &pos);
private:
	GLuint VBO;
	GLuint EBO;
	GLuint lightVAO;
	glm::vec3 color_ = {1.0,1.0,1.0};

};

