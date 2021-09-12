#include "RendererEngine.h"
void RendererEngine::renderTerrian(int width, int height, glm::vec3& cameraPos, glm::mat4 model, glm::mat4 cameraLook, glm::mat4 projection, glm::vec3 lightPos, std::vector<glm::vec3>& lightsPos) {
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderTerrian.Use();
	shaderTerrian.Design(cameraLook, lightsPos);
	shaderTerrian.SetMat4("projection", *this->projection);
	shaderTerrian.SetMat4("view", cameraLook);
	// set light uniforms
	shaderTerrian.SetVec3("viewPos", cameraPos);
	shaderTerrian.SetVec3("lightPos", lightPos);
	shaderTerrian.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
	auto [scaleX, scaleZ] = t->getScaleTerrian();
	float trans = t->getSize() / 2;
	model = glm::translate(model, glm::vec3(-trans, 0, -trans));
	shaderTerrian.SetMat4("model", model);
	t->ActiveTexture();
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	t->Draw();
}