#include "RendererEngine.h"
void RendererEngine::configurateDepthBuf() {
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void RendererEngine::configurateWater(int width, int height, const char* pathDuDvMap) {
	water.Init(width, height);
	water.loadDuDv(pathDuDvMap);
}

void RendererEngine::configurateHDR(int width, int height) {
	width_ = width;
	height_ = height;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);

	//Frame Buffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &textureColorBufferMultiSampled);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width, height, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glGenFramebuffers(1, &intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
	// create a color attachment texture
	glGenTextures(1, &screenTexture);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);
};
void RendererEngine::configureShareds() {

	glGenBuffers(1, &bufferTree);
	//standartShared
	shader.Use();
	shader.SetInt("diffuseTexture", 0);
	shader.SetInt("shadowMap", 1);

	shader.SetVec3("skyColor", 0.7, 0.7, 0.7);

	//shaderTerrian
	shaderTerrian.Use();
	shaderTerrian.SetInt("blendMap", 0);
	shaderTerrian.SetInt("backgroundTexture", 1);
	shaderTerrian.SetInt("rTexture", 2);
	shaderTerrian.SetInt("gTexture", 3);
	shaderTerrian.SetInt("bTexture", 4);

	shaderTerrian.SetVec3("skyColor", 0.3, 0.3, 0.3);

	shaderTerrian.SetInt("shadowMap", 5);
	shaderTerrian.SetVec3("fogColor", 0.3, 0.3, 0.3);


	skyboxShader.Use();
	skyboxShader.SetInt("skybox1", 0);
	skyboxShader.SetInt("skybox2", 1);

	WaterShader.Use();
	WaterShader.SetInt("reflectionTexture", 0);
	WaterShader.SetInt("refractionTexture", 1);
	WaterShader.SetInt("dudvMap", 2);

	TextureGrassShader.Use();
	TextureGrassShader.SetInt("grass", 0);
	TextureGrassShader.SetInt("urandom01", 1);

	OccluderShader.Use();
	OccluderShader.SetVec4("vColor", 1, 0, 0, 1);
}
void RendererEngine::configureSkyBox() {
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	vector<std::string> faces
	{
		"resources/textures/skybox/clouds1_east.bmp",
		"resources/textures/skybox/clouds1_west.bmp",
		"resources/textures/skybox/clouds1_up.bmp",
		"resources/textures/skybox/clouds1_down.bmp",
		"resources/textures/skybox/clouds1_north.bmp",
		"resources/textures/skybox/clouds1_south.bmp",
	};
	cubemapTexture = loadCubemap(faces);
	vector<std::string> facesNight
	{
		"resources/textures/skybox/blue/bkg1_right.png",
		"resources/textures/skybox/blue/bkg1_left.png",
		"resources/textures/skybox/blue/bkg1_top.png",
		"resources/textures/skybox/blue/bkg1_bot.png",
		"resources/textures/skybox/blue/bkg1_front.png",
		"resources/textures/skybox/blue/bkg1_back.png"
	};
	cubemapTexture2 = loadCubemap(facesNight);
}

glm::mat4*& RendererEngine::getProjection() {
	return projection;
}
Water& RendererEngine::getWater() {
	return water;
}



void RendererEngine::renderTerrian(int width, int height, glm::vec3& cameraPos, glm::vec3& cameraFront, glm::mat4& model, glm::mat4& cameraLook, glm::mat4& projection, glm::vec3& lightPos, std::vector<glm::vec3>& lightsPos) {
	
	
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

void RendererEngine::renderTextureGrass(glm::mat4 view, float time) {
	glDisable(GL_CULL_FACE);
	vector<bool> WhoDraw(cell->cellAABBs.size(), false);

	glColorMask(false, false, false, false);
	glDepthMask(GL_FALSE);
	query.start();
	for (int i = 0; i < cell->cellAABBs.size(); i++) {
	
		OccluderShader.Use();
		OccluderShader.SetMat4("view", view);
		OccluderShader.SetMat4("projection", *projection);
		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, cell->cellAABBs.at(i).getPos());
		model = glm::scale(model, cell->cellAABBs.at(i).getScale());
		OccluderShader.SetMat4("model", model);
		AABB::draw();
		if (query.Test() > 10) WhoDraw.at(i) = true;
	}
	query.end();
	glColorMask(true, true, true, true);
	glDepthMask(GL_TRUE);
	TextureGrassShader.Use();
	TextureGrassShader.SetFloat("time", time);
	TextureGrassShader.SetMat4("view", view);
	TextureGrassShader.SetMat4("projection", *projection);
	TextureGrassShader.SetVec3("cameraPos", camera->cameraPos);
	TextureGrassShader.SetVec3("up", camera->cameraUp);
	TextureGrassShader.SetVec3("right", camera->cameraRight());

	for (int i = 0; i < WhoDraw.size(); i++)
	{
			if(WhoDraw.at(i) == true)
			cell->cellGrasses.at(i).draw(*projection,view);
	}	
	glEnable(GL_CULL_FACE);
};

void RendererEngine::EnableHDR() {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	shader.Use();
	shader.SetVec4("plane", glm::vec4(0, -1, 0, 100000));
	shaderTerrian.Use();
	shaderTerrian.SetVec4("plane", glm::vec4(0, -1, 0, 100000));
	objclrShader.Use();
	objclrShader.SetVec4("plane", glm::vec4(0, -1, 0, 100000));
	ShaderNoBone.Use();
	ShaderNoBone.SetVec4("plane", glm::vec4(0, -1, 0, 100000));
}

void RendererEngine::EnableReflectionWater() {
	water.EnableReflection();
	shader.Use();
	shader.SetVec4("plane", glm::vec4(0, 1, 0, -water.getHeight()));
	shaderTerrian.Use();
	shaderTerrian.SetVec4("plane", glm::vec4(0, 1, 0, -water.getHeight()));
	objclrShader.Use();
	objclrShader.SetVec4("plane", glm::vec4(0, 1, 0, -water.getHeight()));
	ShaderNoBone.Use();
	ShaderNoBone.SetVec4("plane", glm::vec4(0, 1, 0, -water.getHeight()));
}

void RendererEngine::EnableRefractionWater() {
	water.EnableRefraction();
	shader.Use();
	shader.SetVec4("plane", glm::vec4(0, -1, 0, water.getHeight()));
	shaderTerrian.Use();
	shaderTerrian.SetVec4("plane", glm::vec4(0, -1, 0, water.getHeight()));
	objclrShader.Use();
	objclrShader.SetVec4("plane", glm::vec4(0, -1, 0, water.getHeight()));
	ShaderNoBone.Use();
	ShaderNoBone.SetVec4("plane", glm::vec4(0, -1, 0, water.getHeight()));
}

void RendererEngine::IdleFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void RendererEngine::DisableHDR() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
	glBlitFramebuffer(0, 0, width_, height_, 0, 0, width_, height_, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RendererEngine::renderScene(int width, int height, glm::vec3& cameraPos, glm::mat4& cameraLook, glm::mat4& projection, glm::vec3 lightPos, unsigned int texture) {
	shader.Use();
	shader.SetMat4("projection", projection);
	shader.SetMat4("view", cameraLook);
	// set light uniforms
	shader.SetVec3("viewPos", cameraPos);
	shader.SetVec3("lightPos", lightPos);
	shader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
	glm::mat4 model = glm::mat4(1.0f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	// cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.2f, 1.5f, 0.5));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetMat4("model", model);
	drawCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.5f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetMat4("model", model);
	drawCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.5f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	shader.SetMat4("model", model);
	drawCube();
	for (auto [name, value] : bodiesMatrixTrans) {

		if (name == "sphere") {
			shader.SetMat4("model", value);
			sphere1.draw();
		}

		if (name == "cylinder") {
			shader.SetMat4("model", value);
			cylinder1.draw();
		}

	}
};

void RendererEngine::renderWater(glm::mat4& cameraLook, float deltaTime, Camera& camera) {
	WaterShader.Use();
	WaterShader.SetMat4("projection", *projection);
	WaterShader.SetMat4("view", cameraLook);
	water.addMoveFactor(deltaTime);
	WaterShader.SetVec3("cameraPosition", camera.cameraPos);
	WaterShader.SetFloat("moveFactor", water.getMoveFactor());
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-40, water.getHeight(), -40));
	model = glm::scale(model, glm::vec3(60, 1, 60));

	WaterShader.SetMat4("model", model);
	glDisable(GL_CULL_FACE);
	water.ActiveTexture();
	water.drawWater();
	glEnable(GL_CULL_FACE);
};
void RendererEngine::renderSkyBox(glm::mat4& view, glm::mat4& projection, float currFrame) {
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader.Use();
	rotation_sky += ROTATION_STEP;
	glm::mat4 my_view = glm::rotate(glm::mat4(glm::mat3(view)), glm::radians(rotation_sky), glm::vec3(0.0, 1.0, 0.0));
	skyboxShader.SetMat4("view", my_view);
	skyboxShader.SetMat4("projection", projection);
	// skybox cube
	time += currFrame / 1000;
	time %= 24000;
	unsigned int texture1;
	int texture2;
	float blendFactor;
	if (time >= 0 && time < 5000) {
		texture1 = cubemapTexture;
		texture2 = cubemapTexture;
		blendFactor = (static_cast<float>(time) - 0.0f) / (5000.0f - 0.0f);
	}
	else if (time >= 5000 && time < 8000) {
		texture1 = cubemapTexture;
		texture2 = cubemapTexture2;
		blendFactor = (static_cast<float>(time) - 5000.0f) / (8000.0f - 5000.0f);
	}
	else if (time >= 8000 && time < 21000) {
		texture1 = cubemapTexture2;
		texture2 = cubemapTexture2;
		blendFactor = (static_cast<float>(time) - 8000.0f) / (21000.0f - 8000.0f);
	}
	else {
		texture1 = cubemapTexture2;
		texture2 = cubemapTexture;
		blendFactor = (static_cast<float>(time) - 21000.0f) / (24000.0f - 21000.0f);
	}

	glBindVertexArray(skyboxVAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture2);
	skyboxShader.SetFloat("blendFactor", blendFactor);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}
void RendererEngine::renderModelAnim(Model* model, glm::mat4& trans, glm::mat4& view, float deltaTime, std::vector<glm::vec3>& lightPos, glm::vec3& cameraPos, glm::vec3& cameraFront) {
	objclrShader.Use();

	objclrShader.Design(view, lightPos, cameraPos, cameraFront);
	objclrShader.SetMat4("projection", *projection);
	objclrShader.SetMat4("view", view);
	objclrShader.SetMat4("model", trans);

	if (!model->anim)    //If the object is rigged...
	{
		for (int i = 0; i < model->meshes.size(); i++) {
			model->meshes[i].bindTexture(objclrShader);
			if (model->meshes[i].sceneLoaderSkeleton.bones.size() > 0) {
				model->meshes[i].SetRigged(objclrShader);
			}
			model->meshes[i].DrawSelf();
		}
	}
	else
	{
		for (int i = 0; i < model->meshes.size(); i++) {
			model->meshes[i].bindTexture(objclrShader);
			model->meshes[i].DrawSelf();
		}
	}

};
void RendererEngine::prepareModelInstanse(vector<glm::mat4>matrix) {
	if (matrix.size() == 0) return;
	glBindBuffer(GL_ARRAY_BUFFER, bufferTree);

	glBufferData(GL_ARRAY_BUFFER, matrix.size() * sizeof(glm::mat4), &matrix[0], GL_DYNAMIC_DRAW);
};

void RendererEngine::renderModel(Model* model, glm::mat4& view, std::vector<glm::vec3>& lightPos, int matrixSize, glm::vec3& cameraPos, glm::vec3& cameraFront) {


	if (matrixSize == 0) return;
	glBindBuffer(GL_ARRAY_BUFFER, bufferTree);
	ShaderNoBone.Use();
	ShaderNoBone.Design(view, lightPos, cameraPos, cameraFront);
	ShaderNoBone.SetMat4("projection", *projection);
	ShaderNoBone.SetMat4("view", view);
	for (int i = 0; i < model->meshes.size(); i++) {
		glBindVertexArray(model->meshes[i].VAO);
		// set attribute pointers for matrix (4 times vec4)
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(8);
		glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));


		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
		glVertexAttribDivisor(8, 1);

		model->meshes[i].bindTexture(ShaderNoBone);
		glBindVertexArray(model->meshes[i].VAO);
		glDrawElementsInstanced(
			GL_TRIANGLES, model->meshes[i]._indices.size(), GL_UNSIGNED_INT, 0, matrixSize);
		glBindVertexArray(0);
	}
};

void RendererEngine::drawCube() {
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void RendererEngine::renderInShadow(glm::vec3& PosLight, glm::vec3& dirLight) {

	float near_plane = 0.1f, far_plane = 110.0f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(PosLight, dirLight, glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	// render scene from light's point of view
	simpleDepthShader.Use();
	simpleDepthShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);
	glm::mat4 model = glm::mat4(1.0f);
	auto [scaleX, scaleZ] = t->getScaleTerrian();
	float trans = t->getSize() / 2;
	model = glm::translate(model, glm::vec3(-trans, 0, -trans));
	simpleDepthShader.SetMat4("model", model);
	glDisable(GL_CULL_FACE);
	t->Draw();
	glEnable(GL_CULL_FACE);
	// cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.2f, 1.5f, 0.5));
	model = glm::scale(model, glm::vec3(0.5f));
	simpleDepthShader.SetMat4("model", model);
	drawCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.5f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	simpleDepthShader.SetMat4("model", model);
	drawCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.5f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	simpleDepthShader.SetMat4("model", model);
	drawCube();
	for (auto [name, value] : bodiesMatrixTrans) {

		if (name == "sphere") {
			simpleDepthShader.SetMat4("model", value);
			sphere1.draw();
		}

		if (name == "cylinder") {
			simpleDepthShader.SetMat4("model", value);
			cylinder1.draw();
		}
	}
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RendererEngine::renderHDR(float epsilon, glm::mat4 model) {
	screen.Use();
	screen.SetFloat("epsilon", epsilon);
	screen.SetMat4("model", model);
	glBindVertexArray(quadVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void RendererEngine::renderReflection(float epsilon, glm::mat4 model) {
	screen.Use();
	screen.SetFloat("epsilon", epsilon);
	screen.SetMat4("model", model);
	glBindVertexArray(quadVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, water.getReflectionTexture());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void RendererEngine::renderRefraction(float epsilon, glm::mat4 model) {
	screen.Use();
	screen.SetFloat("epsilon", epsilon);
	model = glm::scale(model, { 0.25,0.25,1 });
	model = glm::translate(model, { -1,1,1 });

	screen.SetMat4("model", model);
	glBindVertexArray(quadVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, water.getRefractionTexture());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RendererEngine::destroy() {
	FileLoader::DELETEFILE();
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	water.WaterDestroy();
	sphere1.Delete();
	cylinder1.Delete();
	for (auto& a :cell->cellGrasses) {
		a.Delete();
	};
	glDeleteBuffers(1, &bufferTree);

}