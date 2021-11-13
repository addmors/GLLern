#pragma once
#include "../shader/Shad.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static float CalcRadiusLightSphere(float constant, float linear, float quadratic, glm::vec3 specular) {
	float lightMax = std::fmaxf(std::fmaxf(specular.r, specular.g), specular.b);
	return (-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * lightMax))) / (2 * quadratic);
}

struct PointLight
{

	glm::vec3 main_pos;
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
	float radius_sphere;

	PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, 
				float constant, float linear, float quadratic):
		main_pos(position),
		position(position),
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		constant(constant),
		linear(linear),
		quadratic(quadratic),
		radius_sphere(CalcRadiusLightSphere(constant, linear, quadratic, diffuse))
	{};
	float getRadius() { return radius_sphere; };
	glm::vec3& getPosition() { return position; };
	glm::vec3& getMainPosition() { return main_pos; };

	glm::vec3& getDiffuse() { return diffuse; };
	void AddPointInShader(Shader* shader) {
		shader->SetVec3("pointLight.position", position);
		shader->SetVec3("pointLight.ambient", ambient);
		shader->SetVec3("pointLight.diffuse", diffuse);
		shader->SetVec3("pointLight.specular", specular);
		shader->SetFloat("pointLight.constant", constant);
		shader->SetFloat("pointLight.linear", linear);
		shader->SetFloat("pointLight.quadratic", quadratic);
	}
};

class PointsLight {
protected:
	std::vector<PointLight> pointsLight;
public:
	void AddPoint(PointLight point) {
		pointsLight.push_back(point);
	}
	void DeleteLast() {
		if (!pointsLight.empty())
		{
			pointsLight.pop_back();
		}
		else {
			std::cout << "Vector is empty";
		};
	}
	std::vector<PointLight>& getPointsLight(){ return pointsLight; };
};



class light
{
public:
	void Draw();
	
	void Delete() { (glDeleteVertexArrays(1, &lightVAO)); 
					glDeleteBuffers(1, &VBO);
					glDeleteBuffers(1, &EBO);
					}
	light(GLfloat* vertices_start, GLfloat* vertices_end,
		GLuint* indices_start, GLuint* indices_end);
private:
	GLuint VBO;
	GLuint EBO;
	GLuint lightVAO;
	glm::vec3 color_ = {.0,1.0,1.0};

};

