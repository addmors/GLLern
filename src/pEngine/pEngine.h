#pragma once
#include<vector>
#include <memory>
#include<btBulletDynamicsCommon.h>
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tuple>
#include<list>
#include <iterator>
#include <map>
#include <string>
class Sphere;
class Cylinder;
class Terrian;

using mBodyParam = std::tuple<
	std::shared_ptr<btRigidBody>,
	std::shared_ptr<btMotionState>,
	std::shared_ptr<btCollisionShape>>;



class pEngine
{
public:

	pEngine();
	~pEngine();

	void step(float delta);

	btDynamicsWorld* getWorld() { return world.get();}
	btBroadphaseInterface* getBroadPhase() { return broadPhase.get(); }
	auto& getBody() { return bodies; }

	btRigidBody* addTerrian(Terrian* terrian);
	btRigidBody* addSphere(float rad, float x, float y, float z, float mass);
	btRigidBody* addCylinder(float d, float h, float x, float y, float z, float mass);
	btRigidBody* addBox(float height, float widht, float length, float x, float y, float z, float mass);

	glm::mat4 getmatSphere(btRigidBody* sphere);
	glm::mat4 getmatCylinder(btRigidBody* cylinder);
	glm::mat4 getmatBox(btRigidBody* box);
	std::pair < glm::vec3, glm::vec3 > getAABB(btRigidBody* body);
	
	std::vector<size_t> genMatrices(std::map<std::string, std::vector<glm::mat4>::iterator>& Instance);

private:
	std::shared_ptr<btDynamicsWorld> world;
	std::shared_ptr<btCollisionDispatcher> dispather;
	std::shared_ptr<btBroadphaseInterface> broadPhase;
	std::shared_ptr<btConstraintSolver> solver;
	std::shared_ptr<btCollisionConfiguration> collisionConfig;
	std::list<mBodyParam> bodies;
	float sizePlane = 128.0f;
};

