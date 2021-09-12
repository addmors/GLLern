#define GLEW_STATIC
#include <string>
#include <set>
#include <iostream>
#include <memory>
#include<iterator>
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include "Precompile.h"
#include"shader/Shad.h"
#include "RendererEngine.h"
#include "light/light.h"
#include"Camera/Camera.h"
#include"Model/Model.h"
#include"Loader.h"
#include "Player\Player.h"
#include "pEngine\pEngine.h"
#include <AntTweakBar.h>
#include "Terrian.h"

glm::vec3 skaling = glm::vec3(0.1f, 0.1f, 0.1f);
glm::vec3 cameraPos = glm::vec3(3.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(-1.0f, -0.0f, -0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat deltaTime = 0.0f;	// Время, прошедшее между последним и текущим кадром
GLfloat lastFrame = 0.0f;  	// Время вывода последнего кадра
GLfloat yaw = -135.0f;
GLfloat pitch = 0.0f;
GLfloat fov = 45.0f;
GLfloat lastX = 400, lastY = 300; //Координаты мышки;
float epsilon = 2.4;

TwType TW_TYPE_OGLDEV_VECTOR3F;
TwType TW_TYPE_OGLDEV_ATTENUATION;

Camera camera(cameraPos, cameraFront, cameraUp, fov);
std::unique_ptr<pEngine> pengine = std::make_unique<pEngine>();
Model ourModel;
GLFWwindow* window;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback_for_movement(int key, int action);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
float inline randFloat(float LO, float HI) {
	return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
};
static int OgldevKeyToATBKey(int OgldevKey);

bool CameraMove = false;

void TW_CALL cameraMoveCB(void* p)
{
	CameraMove = !CameraMove;
}

void mouseKey(GLFWwindow* window, int button, int action, int mode)
{
	TwMouseButtonID btn = (button == GLFW_MOUSE_BUTTON_LEFT) ? TW_MOUSE_LEFT : TW_MOUSE_RIGHT;
	TwMouseAction ma = (action == GLFW_PRESS) ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED;
	TwMouseButton(ma, btn);
};

glm::mat4 getInbtTransform(btTransform& t);

unsigned int planeVAO;

int main() {
	
	TwBar* bar;

	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//Минорная
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Установка профайла для которого создается контекст
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Выключение возможности изменения размера окна
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(1920, 1080, "LearnOpenGL", nullptr/*glfwGetPrimaryMonitor()*/, nullptr);
	if (window == nullptr)
	{
		fprintf(stderr,"Failed to create GLFW window");	
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW");
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPos(window, lastX, lastY); TwDefine(" TweakBar size='200 400' color='96 216 224' "); // change default tweak bar size and color

	glfwSetScrollCallback(window, scroll_callback); //активаци колесика мыши
	glfwSetKeyCallback(window, key_callback);// функции по нажатию.
	glfwSetCursorPosCallback(window, mouse_callback); //Функции по движению мыши
	glfwSetMouseButtonCallback(window, mouseKey);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Активация курсора скрыть указатель 
	
	
	int width = 1920, height = 1080;
	glfwGetFramebufferSize(window, &width, &height);	
	
	TwWindowSize(width, height);
	// Initialize AntTweakBar
	TwInit(TW_OPENGL_CORE, NULL);

	// Create a tweak bar
	bar = TwNewBar("TweakBar");
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLFW and OpenGL.' "); // Message added to the help bar.
	TwDefine(" TweakBar size='200 400' color='255 0 0' "); // change default tweak bar size and color
	
	TwStructMember Vector3fMembers[] = {
	{ "x", TW_TYPE_FLOAT, offsetof(glm::vec3, x), "" },
	{ "y", TW_TYPE_FLOAT, offsetof(glm::vec3, y), "" },
	{ "z", TW_TYPE_FLOAT, offsetof(glm::vec3, z), "" }
	};

	vector<glm::vec3> lightPos;
	glm::vec3 lightPosForShad(0.01f, 7.0f, 0.0f);
	glm::vec3 dirLight(0, -1, 0);

	lightPos.push_back(glm::vec3(4.0f, 4.0f, 4.0f));
	lightPos.push_back(glm::vec3(-4.0f, 4.0f, 4.0f));
	lightPos.push_back(glm::vec3(4.0f, 4.0f, -4.0f));
	lightPos.push_back(glm::vec3(-4.0f, 4.0f, -4.0f));

	TW_TYPE_OGLDEV_VECTOR3F = TwDefineStruct("Vector3f", Vector3fMembers, 3, sizeof(glm::vec3), NULL, NULL);

	TwAddButton(bar, "Camera", NULL, NULL, "");
	TwAddVarRW(bar, "Position", TW_TYPE_OGLDEV_VECTOR3F, (void*)&dirLight, NULL);
	TwAddVarRW(bar, "DirectionCamera", TW_TYPE_DIR3F, (void*)&camera.cameraFront, NULL);
	TwAddVarRW(bar, "DirectionLght", TW_TYPE_DIR3F, (void*)&dirLight, "axisx=x  axisy=y axisz=z");
	TwAddVarRW(bar, "Epsilon", TW_TYPE_FLOAT, &epsilon, "min=0 max=5 step=0.1");

	TwAddButton(bar, "AutoRotate", cameraMoveCB, NULL, " label='CameraMove' ");
	
	
	Shader lightShader("src/shader/shader.vs", "src/shader/lampshad.fs");
	//Shader objclrShader("src/shader/shader3.vs", "src/shader/objvithcolor.fs");
	
	unsigned int woodTexture = loadTexture("Cyrcle.png");
	unsigned int grassTexture = loadTexture("grass.png");

	ourModel.loadModel("resources/objects/Black Dragon NEW/ganfaul_m_aure.dae");

	ourModel.loadIdleAnimaitons("resources/objects/Black Dragon NEW/idle.dae");
	ourModel.loadAnims("resources/objects/Black Dragon NEW/Walking.dae","Walking");
	ourModel.loadAnims("resources/objects/Black Dragon NEW/Walking Backwards.dae","WalkBackWard");


	Player persone(camera, pengine.get());
	//get key
	camera.keys = &Key.keys;
	ourModel.keys = &Key.keys;
	persone.keys = &Key.keys;
	RendererEngine renderer;
	Terrian terrain = Terrian(32768/64, "heightmap6.png");
	terrain.loadTextures("blendMap.png", "rTexture.png", "grass2.png", "bTexture.png", "grass.png");
	

	Model Modeltree;
	Modeltree.loadModel("resources/objects/tree/Tree.obj");

	std::vector<glm::vec3> positions;
	for (int i = 0; i < 10; i++) {
		glm::mat4 model = glm::mat4(1.0f);
		float x = i*2;
		float z = 0;
		float y = terrain.getHeightOfTerrian(x, z);
		positions.push_back({ x,y,z });

		pengine->addCylinder(2, 5, x, y, z, 0);
	}
	for (int i = 0; i < 10; i++) {
		glm::mat4 model = glm::mat4(1.0f);
		float x = i*2;
		float z = 30;
		float y = terrain.getHeightOfTerrian(x, z);
		positions.push_back({ x,y,z });

		pengine->addCylinder(2, 5, x, y, z, 0);
	}
	for (int i = 0; i < 10; i++) {
		glm::mat4 model = glm::mat4(1.0f);
		float x = 0;
		float z = i*2;
		float y = terrain.getHeightOfTerrian(x, z);
		positions.push_back({ x,y,z });

		pengine->addCylinder(2, 5, x, y, z, 0);
	}
	for (int i = 0; i < 10; i++) {
		glm::mat4 model = glm::mat4(1.0f);
		float x = 30;
		float z = i*2;
		float y = terrain.getHeightOfTerrian(x, z);
		positions.push_back({ x,y,z });

		pengine->addCylinder(2, 5, x, y, z, 0);
	}
	

	renderer.t = &terrain;

	//create Shaders
	// -----------------------
	renderer.configureShareds();

	// configure depth map FBO
	// -----------------------
	renderer.configurateDepthBuf();

	// skybox configuration
	// --------------------
	renderer.configureSkyBox();

	// shader configuration
	// --------------------
	renderer.configurateHDR(width, height);
	

	// water configuration
	// --------------------
	renderer.configurateWater(width, height, "dudvmap.png");
	
	//Frame texture	
	//End Frame buffer;


	light Light(std::begin(vertices),std::end(vertices), std::begin(indices),std::end(indices), lightPos);

	//pengine->addCylinder(2, 5, 0, 30, 0, 1.0);
	//pengine->addSphere(1.0,0,25,0,1.0);
	
	auto terrianBody = pengine->addTerrian(&terrain);
	lastFrame = glfwGetTime();
	glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)width / (float)height, 0.1f, 400.0f);
	renderer.getProjection() = &projection;
	while (!glfwWindowShouldClose(window))
	{
		glm::mat4 model = glm::mat4();
		
		GLfloat currentFrame = glfwGetTime();
		
		float deltaTime = currentFrame - lastFrame;
		
		lastFrame = currentFrame;
		
		pengine->step(deltaTime);
		
		camera.deltaTime = deltaTime;
		camera.do_movement();

		glm::mat4 view = camera.LoocAt();
		btVector3 position = persone.charCon->getGhostObject()->getWorldTransform().getOrigin();
		

		float minHeight = terrain.getHeightOfTerrian(position.x(), position.z()) - 1;
		
		if (position.y() < minHeight-5) {
			btTransform matPos;
			matPos.setIdentity();
			matPos.setOrigin({ position.x(),minHeight+5,position.z() });
			persone.charCon->getGhostObject()->setWorldTransform(matPos);
		}
		lightPosForShad = { position.x(), position.y(), position.z() };

		lightPos.back() = lightPosForShad - dirLight * 70.0f;
		
		btTransform t;
		renderer.bodiesMatrixTrans.resize(pengine->getBody().size());
		auto s = renderer.bodiesMatrixTrans.begin();
		for (auto [a, b, c] : pengine->getBody()) {
			if (a->getInvMass() != 0) {
				if (a->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE) {
					auto pr = std::make_pair("sphere", pengine->getmatSphere(a.get()));
					*(s) = pr;
				}
				else if (a->getCollisionShape()->getShapeType() == CYLINDER_SHAPE_PROXYTYPE) {
					*(s) = std::make_pair("cylinder", std::move(pengine->getmatCylinder(a.get())));
				}
				else *(s) = std::make_pair("unknownobj", glm::mat4());
			}
			s++;
		}

		

		persone.update(deltaTime);
		ourModel.PlayAnimation(deltaTime);
		ourModel.skel.Update();
		for (Mesh& mesh : ourModel.meshes) {
			mesh.UpdateBoneMat();
		};
		glm::mat4 glmatrix = getInbtTransform(persone.charCon->getGhostObject()->getWorldTransform());
		glmatrix = glm::rotate(glmatrix, glm::radians(90.0f - camera.yaw), glm::vec3(0.0, 1.0, 0.0));
		glmatrix = glm::translate(glmatrix, glm::vec3(0, 0, -1));
		
		
		std::vector<glm::mat4> modelMatrices;
		for (auto& a : positions) {
			glm::vec4 answer = projection * view * glm::vec4(a, 1.0);
			answer /= answer.w;
			if (abs(answer.x) > 1.1 || abs(answer.y) > 1.1 || abs(answer.z) > 1)
				continue;
			else {
				glm::mat4 matrix = glm::mat4(1);
				matrix = glm::translate(matrix, a);
				modelMatrices.push_back(matrix);
			}
		};
		renderer.prepareModelInstanse(modelMatrices);
		
		
		renderer.renderInShadow(lightPos.back(), lightPosForShad);
		glEnable(GL_CLIP_DISTANCE0);
		
		float distance = 2 * (camera.cameraPos.y - renderer.getWater().getHeight());
		camera.cameraPos.y -= distance;
		camera.pitch = -camera.pitch;	
		view = camera.LoocAt();

		
		renderer.EnableReflectionWater();
		renderer.renderTerrian(width, height, camera.cameraPos, model, view, projection, lightPos.back(), lightPos);
		renderer.renderScene(width, height, camera.cameraPos, view, projection, lightPos.back(), woodTexture);
		renderer.renderModel(&Modeltree, view, lightPos, modelMatrices.size());
		renderer.renderModelAnim(&ourModel, glmatrix, view, deltaTime, lightPos);
		renderer.renderSkyBox(view, projection, currentFrame);
		
		camera.cameraPos.y += distance;
		camera.pitch = -camera.pitch;
		view = camera.LoocAt();

		renderer.EnableRefractionWater();
		renderer.renderTerrian(width, height, camera.cameraPos, model, view, projection, lightPos.back(), lightPos);
		renderer.renderScene(width, height, camera.cameraPos, view, projection, lightPos.back(), woodTexture);
		renderer.renderModel(&Modeltree, view, lightPos, modelMatrices.size());
		renderer.renderModelAnim(&ourModel, glmatrix, view, deltaTime, lightPos);
		renderer.renderSkyBox(view, projection, currentFrame);

		

		glDisable(GL_CLIP_DISTANCE0);
		
		renderer.EnableHDR();
		renderer.renderTerrian(width, height, camera.cameraPos,model, view, projection, lightPos.back(), lightPos);
		Light.UseLight(view, projection, lightPos);
		renderer.renderWater(view, deltaTime, camera);
		renderer.renderScene(width, height, camera.cameraPos, view, projection, lightPos.back(), woodTexture);
		renderer.renderModel(&Modeltree, view, lightPos, modelMatrices.size());
		renderer.renderModelAnim(&ourModel,glmatrix, view, deltaTime, lightPos);
		renderer.renderSkyBox(view, projection, currentFrame);
		renderer.DisableHDR();
		
		renderer.renderHDR(epsilon);
		//renderer.renderReflection(epsilon);
		TwDraw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Light.Delete();
	renderer.destroy();
	persone.destroy();
	for (auto& a : ourModel.meshes) {
		a.Delete();
	}
	TwTerminate();
	glfwTerminate();
	return 0;

}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
	// и приложение после этого закроется
	int ATBKey = OgldevKeyToATBKey(key);
	cout << "action = " <<action << "srsr";
	if (ATBKey != TW_KEY_LAST && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
		TwKeyPressed(ATBKey, TW_KMOD_NONE);
	};
	if (Key.keys[GLFW_KEY_LEFT_SHIFT] && key == GLFW_KEY_K && action == GLFW_PRESS)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (Key.keys[GLFW_KEY_LEFT_SHIFT] && key == GLFW_KEY_O && action == GLFW_PRESS)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_C && action == GLFW_PRESS)  epsilon += 0.1;
	if (key == GLFW_KEY_V && action == GLFW_PRESS)  epsilon -= 0.1;

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
 		auto sphere = pengine->addSphere(1,camera.objPos.x+camera.cameraFront.x, camera.objPos.y+camera.cameraFront.y, camera.objPos.z+camera.cameraFront.z,1);
		sphere->setLinearVelocity(btVector3(camera.cameraFront.x*20, camera.cameraFront.y*20, camera.cameraFront.z*20));
	}

	key_callback_for_movement(key, action);
	ourModel.key_callback(key, action);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	TwMouseMotion(xpos, ypos);
	if(CameraMove)
		camera.mouse_callback(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (Key.keys[GLFW_KEY_C])  skaling += (yoffset * 0.1);
	else camera.scroll_callback(xoffset, yoffset);
}

void key_callback_for_movement(int key, int action) {
	Key.keys[key] = action;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


glm::mat4 getInbtTransform(btTransform& t) {
	float mat[16];
	t.getOpenGLMatrix(mat);
	return { mat[0], mat[1], mat[2], mat[3],
		mat[4], mat[5], mat[6], mat[7],
		mat[8], mat[9], mat[10], mat[11],
		mat[12], mat[13], mat[14], mat[15]};
}

static int OgldevKeyToATBKey(int OgldevKey)
{
	if (OgldevKey >= GLFW_KEY_SPACE && OgldevKey <= GLFW_KEY_RIGHT_BRACKET) {
		return OgldevKey;
	}
	switch (OgldevKey) {
	case GLFW_KEY_BACKSPACE:
		return TW_KEY_BACKSPACE;
	case GLFW_KEY_TAB:
		return TW_KEY_TAB;
		//            return TW_KEY_CLEAR;
	case GLFW_KEY_ENTER:
		return TW_KEY_RETURN;
		//return TW_KEY_PAUSE;
	case GLFW_KEY_ESCAPE:
		return TW_KEY_ESCAPE;
	case GLFW_KEY_DELETE:
		return TW_KEY_DELETE;
	case GLFW_KEY_UP:
		return TW_KEY_UP;
	case GLFW_KEY_DOWN:
		return TW_KEY_DOWN;
	case GLFW_KEY_RIGHT:
		return TW_KEY_RIGHT;
	case GLFW_KEY_LEFT:
		return TW_KEY_LEFT;
	case GLFW_KEY_INSERT:
		return TW_KEY_INSERT;
	case GLFW_KEY_HOME:
		return TW_KEY_HOME;
	case GLFW_KEY_END:
		return TW_KEY_END;
	case GLFW_KEY_PAGE_UP:
		return TW_KEY_PAGE_UP;
	case GLFW_KEY_PAGE_DOWN:
		return TW_KEY_PAGE_DOWN;
	case GLFW_KEY_F1:
		return TW_KEY_F1;
	case GLFW_KEY_F2:
		return TW_KEY_F2;
	case GLFW_KEY_F3:
		return TW_KEY_F3;
	case GLFW_KEY_F4:
		return TW_KEY_F4;
	case GLFW_KEY_F5:
		return TW_KEY_F5;
	case GLFW_KEY_F6:
		return TW_KEY_F6;
	case GLFW_KEY_F7:
		return TW_KEY_F7;
	case GLFW_KEY_F8:
		return TW_KEY_F8;
	case GLFW_KEY_F9:
		return TW_KEY_F9;
	case GLFW_KEY_F10:
		return TW_KEY_F10;
	case GLFW_KEY_F11:
		return TW_KEY_F11;
	case GLFW_KEY_F12:
		return TW_KEY_F12;
	default:
		return -1;
	}

	return TW_KEY_LAST;
}

