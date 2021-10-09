#include "RendererEngine.h"
void RendererEngine::renderTerrian(int width, int height, glm::vec3& cameraPos, glm::vec3& cameraFront, glm::mat4& model, glm::mat4& cameraLook, glm::mat4& projection, glm::vec3& lightPos, std::vector<glm::vec3>& lightsPos) {
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderTerrian.Use();
	shaderTerrian.Design(cameraLook, lightsPos, cameraPos, cameraFront);
	shaderTerrian.SetMat4("projection", *this->projection);
	shaderTerrian.SetMat4("view", cameraLook);
	// set light uniforms
	shaderTerrian.SetVec3("viewPos", cameraPos);
	shaderTerrian.SetVec3("lightPos", lightPos);
	shaderTerrian.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
	shaderTerrian.SetMat4("model", model);
	t->ActiveTexture();
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	t->Draw();
};

void RendererEngine::renderTextureGrass(glm::mat4 view, std::vector<glm::vec3>& pos) {
	TextureGrassShader.Use();


	TextureGrassShader.SetMat4("view", view);

	TextureGrassShader.SetMat4("projection", *projection);
	TextureGrassShader.SetMat4("projection", *projection);
	TextureGrassShader.SetVec3("up", camera->cameraUp);
	TextureGrassShader.SetVec3("right", camera->cameraRight());

	for (auto& a : pos) {
		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, a);
		TextureGrassShader.SetMat4("model", model);
		texGrass->draw(*projection, view);
	}
};