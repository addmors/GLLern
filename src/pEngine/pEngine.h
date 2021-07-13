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

	void step(float delta) {
		if (world != nullptr)  world->stepSimulation(delta);
		auto curr = bodies.begin();
		while (curr != bodies.end()) {
			auto nextBody = std::next(curr);
			btVector3 t = std::get<0>(*curr)->getWorldTransform().getOrigin();
			if(abs(t.x()) > sizePlane || abs(t.z()) > sizePlane) {
				world->removeRigidBody(&*std::get<0>(*curr));
				bodies.erase(curr);
			}
			curr = nextBody;
		}

		/*for (int i = 0; i < bodies.size(); i++) {
			std::get<0>(bodies.at(i))->setGravity({ 0,-delta * 512,0 });
;			btVector3 t = std::get<0>(bodies.at(i))->getWorldTransform().getOrigin();
			if (abs(t.x()) > sizePlane || abs(t.z()) > sizePlane) {
				world->removeRigidBody(std::get<0>(bodies.at(i)).get());
				bodies.erase(bodies.begin()+i);
			}
		}*/
	}
	btDynamicsWorld* getWorld() { return world.get();}
	btBroadphaseInterface* getBroadPhase() { return broadPhase.get(); }
	

	btRigidBody* addTerrian(Terrian* terrian);

	glm::mat4 getmatSphere(btRigidBody* sphere);

	glm::mat4 getmatCylinder(btRigidBody* cylinder);

	auto& getBody() { return bodies; }

	btRigidBody* addSphere(float rad, float x, float y, float z, float mass) {
		btTransform t;
		t.setIdentity();
		t.setOrigin({ x,y,z });
		auto sphere = std::make_shared<btSphereShape>(rad);
		btVector3 inertia(0, 0, 0);
		if (mass != 0.0) {
			sphere->calculateLocalInertia(mass, inertia);
		}
		std::shared_ptr<btMotionState> motion = std::make_shared<btDefaultMotionState>(t);

		btRigidBody::btRigidBodyConstructionInfo info(mass, motion.get(), sphere.get(), inertia);

		auto body = std::make_shared<btRigidBody>(info);

		world->addRigidBody(body.get());
		bodies.push_back(std::make_tuple(body, motion, sphere));
		return body.get();
	}
	btRigidBody* addCylinder(float d, float h, float x, float y, float z, float mass) {
		btTransform t;
		t.setIdentity();
		t.setOrigin({ x,y,z });
		btVector3 inertial;
		std::shared_ptr<btCylinderShape> cyrcle = std::make_shared<btCylinderShape>(btVector3(d / 2.0, h / 2.0, d / 2.0));
		if (mass != 0) {
			cyrcle->calculateLocalInertia(mass, inertial);
		}
		std::shared_ptr<btMotionState> motion = std::make_shared<btDefaultMotionState>(t);
		btRigidBody::btRigidBodyConstructionInfo info(mass, motion.get(), cyrcle.get(), inertial);
		
		std::shared_ptr<btRigidBody> body = std::make_shared<btRigidBody>(info);
		body->getWorldTransform().getOrigin();
		world->addRigidBody(body.get());
		bodies.push_back(std::make_tuple(body,motion,cyrcle));

		return body.get();
	}

private:
	std::shared_ptr<btDynamicsWorld> world;
	std::shared_ptr<btDispatcher> dispather;
	std::shared_ptr<btBroadphaseInterface> broadPhase;
	std::shared_ptr<btConstraintSolver> solver;
	std::shared_ptr<btCollisionConfiguration> collisionConfig;
	std::list<mBodyParam> bodies;
	float sizePlane = 128.0f;
};

