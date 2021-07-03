#define GLEW_STATIC
#include <string>
#include <set>
#include <iostream>
#include <memory>
#include<iterator>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include <stb_image.h>
#include "Precompile.h"
#include"shader/Shad.h"
#include "RendererEngine.h"
#include "light/light.h"
#include"Camera/Camera.h"
#include"Model/Model.h"
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
float epsilon = 0.4;

TwType TW_TYPE_OGLDEV_VECTOR3F;
TwType TW_TYPE_OGLDEV_ATTENUATION;

Camera camera(cameraPos, cameraFront, cameraUp, fov);
std::unique_ptr<pEngine> pengine = std::make_unique<pEngine>();
Model ourModel;
GLFWwindow* window;

void key_callback_for_movement(int key, int action);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
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

unsigned int loadTexture(const char* path);
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

	
	glfwSetCursorPos(window, lastX, lastY); TwDefine(" TweakBar size='200 400' color='96 216 224' "); // change default tweak bar size and color

	glfwSetScrollCallback(window, scroll_callback); //активаци колесика мыши
	glfwSetKeyCallback(window, key_callback);// функции по нажатию.
	glfwSetCursorPosCallback(window, mouse_callback); //Функции по движению мыши
	glfwSetMouseButtonCallback(window, mouseKey);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Активация курсора скрыть указатель 
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);	
	glViewport(0, 0, width, height);

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

	lightPos.push_back(glm::vec3(4.0f, 4.0f, 4.0f));
	lightPos.push_back(glm::vec3(-4.0f, 4.0f, 4.0f));
	lightPos.push_back(glm::vec3(4.0f, 4.0f, -4.0f));
	lightPos.push_back(glm::vec3(-4.0f, 4.0f, -4.0f));

	TW_TYPE_OGLDEV_VECTOR3F = TwDefineStruct("Vector3f", Vector3fMembers, 3, sizeof(glm::vec3), NULL, NULL);

	TwAddButton(bar, "Camera", NULL, NULL, "");
	TwAddVarRW(bar, "Position", TW_TYPE_OGLDEV_VECTOR3F, (void*)&lightPosForShad, NULL);
	TwAddVarRW(bar, "DirectionCamera", TW_TYPE_DIR3F, (void*)&camera.cameraFront, NULL);
	TwAddVarRW(bar, "DirectionLght", TW_TYPE_DIR3F, (void*)&lightPosForShad, "axisx=-x  axisy=-y axisz=-z");
	TwAddVarRW(bar, "Epsilon", TW_TYPE_FLOAT, &epsilon, "min=0 max=5 step=0.1");

	TwAddButton(bar, "AutoRotate", cameraMoveCB, NULL, " label='CameraMove' ");
	
	Shader shader("src/shader/3.1.2.shadow_mapping.vs", "src/shader/3.1.2.shadow_mapping.fs");
	Shader shaderTerrian("src/shader/terrianShader.vs", "src/shader/terrianShader.fs");
	Shader simpleDepthShader("src/shader/3.1.2.shadow_mapping_depth.vs", "src/shader/3.1.2.shadow_mapping_depth.fs");
	Shader lightShader("src/shader/shader.vs", "src/shader/lampshad.fs");
	Shader objclrShader("src/shader/shader3.vs", "src/shader/objvithcolor.fs");
	Shader screen("src/shader/frameshader.vs", "src/shader/frameshader.fs");

	
	unsigned int woodTexture = loadTexture("rook.png");
	unsigned int grassTexture = loadTexture("grass.png");
	ourModel.loadModel("resources/objects/Black Dragon NEW/ganfaul_m_aure.dae");

	ourModel.loadIdleAnimaitons("resources/objects/Black Dragon NEW/idle.dae");
	ourModel.loadAnims("resources/objects/Black Dragon NEW/Walking.dae","Walking");
	ourModel.loadAnims("resources/objects/Black Dragon NEW/Walking Backwards.dae","WalkBackWard");


	Player persone(ourModel, camera, objclrShader, pengine.get());
	//get key
	camera.keys = &Key.keys;
	ourModel.keys = &Key.keys;
	persone.keys = &Key.keys;
	RendererEngine renderer;
	Terrian terrain = Terrian(256, "height.bmp");
	renderer.t = &terrain;

	// configure depth map FBO
	// -----------------------
	renderer.configurateDepthBuf();

	// shader configuration
	// --------------------
	shader.Use();
	shader.SetInt("diffuseTexture", 0);
	shader.SetInt("shadowMap", 1);

	shaderTerrian.Use();
	shaderTerrian.SetInt("diffuseTexture", 0);
	shaderTerrian.SetInt("shadowMap", 1);
	
	renderer.configurateHDR(width, height);
	//Frame texture	
	//End Frame buffer;


	light Light(std::begin(vertices),std::end(vertices), std::begin(indices),std::end(indices), lightPos);

	pengine->addCylinder(2, 5, 0, 30, 0, 1.0);
	pengine->addSphere(1.0,0,20,0,1.0);
	
	auto terrianBody = pengine->addTerrian(&terrain);

	while (!glfwWindowShouldClose(window))
	{
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		glm::mat4 model = glm::mat4();
		GLfloat currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		pengine->step(deltaTime);
		camera.deltaTime = deltaTime;
		lastFrame = currentFrame;
		camera.do_movement();

		glm::mat4 view = camera.LoocAt();

		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)width / (float)height, 0.1f, 100.0f);
		renderer.renderInShadow(lightPosForShad, &simpleDepthShader);
		renderer.EnableHDR();
		
		btTransform t;
		//terrianBody->getMotionState()->getWorldTransform(t);
		//model = getInbtTransform(t);
		renderer.bodiesMatrixTrans.resize(pengine->getBody().size());
		auto s = renderer.bodiesMatrixTrans.begin();
		for (auto [a, b, c] : pengine->getBody()) {
			if (a->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE) {
				auto pr = std::make_pair("sphere", pengine->getmatSphere(a.get()));
				*(s) = pr;
			}
			else if (a->getCollisionShape()->getShapeType() == CYLINDER_SHAPE_PROXYTYPE) {
				*(s) = std::make_pair("cylinder", std::move(pengine->getmatCylinder(a.get())));
			}
			else *(s) = std::make_pair("unknownobj", glm::mat4());
			s++;
		}


		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer.renderTerrian(width, height, &shaderTerrian, camera.cameraPos,model, view, projection, lightPosForShad, grassTexture);
		renderer.renderScene(width, height, &shader, camera.cameraPos, view, projection, lightPosForShad, woodTexture);

		objclrShader.Use();
		view = camera.LoocAt();
		t = persone.charCon->getGhostObject()->getWorldTransform();
		glm::mat4 glmatrix = getInbtTransform(t);
		//model = glm::translate(model, glm::vec3(camera.objPos.x,0, camera.objPos.z)); // translate it down so it's at the center of the scene
		//if(Key.keys[67])
			glmatrix = glm::rotate(glmatrix, glm::radians(90.0f - camera.yaw), glm::vec3(0.0, 1.0, 0.0));

		//model = glm::scale(model,skaling);

		objclrShader.Design(view, lightPos);
		
		objclrShader.SetMat4("projection", projection);
		objclrShader.SetMat4("view", view);
		objclrShader.SetMat4("model", glmatrix);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		persone.update();
		persone.show();
		persone.Persone->PlayAnimation(deltaTime);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		Light.UseLight(view, projection, lightPos);	

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // возвращаем буфер кадра по умолчанию
		
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		renderer.DisableHDR();
		renderer.renderHDR(&screen, epsilon);

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
	if (Key.keys[67])  skaling += (yoffset * 0.1);
	else camera.scroll_callback(xoffset, yoffset);
}

void key_callback_for_movement(int key, int action) {
	Key.keys[key] = action;
};

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
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

