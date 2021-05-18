#pragma once
#include<vector>
#include<btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sphere;
class Cylinder;
class pEngine
{
public:

	pEngine();
	~pEngine();

	void step(float delta) {
		if (world != nullptr)  world->stepSimulation(delta);
	}
	btDynamicsWorld*& getWorld() {
		return world;
	}
	btBroadphaseInterface* getBroadPhase() { return broadPhase; }
	void RenderSphere(btRigidBody* sphere, Sphere* sphereRen, glm::mat4 view, glm::mat4 projectoin);
	void RenderCylinder(btRigidBody* cylinder, Cylinder* cylinderRen, glm::mat4 view, glm::mat4 projectoin);
	std::vector<btRigidBody*>& getBody() { return bodies; }
	btRigidBody* addSphere(float rad, float x, float y, float z, float mass) {
		btTransform t;
		t.setIdentity();
		t.setOrigin({ x,y,z });
		btSphereShape* sphere = new btSphereShape(rad);
		btVector3 inertia(0, 0, 0);
		if (mass != 0.0) {
			sphere->calculateLocalInertia(mass, inertia);
		}
		btMotionState* motion = new btDefaultMotionState(t);

		btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere, inertia);

		btRigidBody* body = new btRigidBody(info);
		world->addRigidBody(body);
		bodies.push_back(body);
		return body;
	}
	btRigidBody* addCylinder(float d, float h, float x, float y, float z, float mass) {
		btTransform t;
		t.setIdentity();
		t.setOrigin({ x,y,z });
		btVector3 inertial;
		btCylinderShape* cyrcle = new btCylinderShape(btVector3(d / 2.0, h / 2.0, d / 2.0));
		if (mass != 0) {
			cyrcle->calculateLocalInertia(mass, inertial);
		}
		btMotionState* motion = new btDefaultMotionState(t);
		btRigidBody::btRigidBodyConstructionInfo info(mass, motion, cyrcle, inertial);

		btRigidBody* body = new btRigidBody(info);
		world->addRigidBody(body);
		bodies.push_back(body);
		return body;
	}

private:
	btDynamicsWorld* world;
	btDispatcher* dispather;
	btBroadphaseInterface* broadPhase;
	btConstraintSolver* solver;
	btCollisionConfiguration* collisionConfig;
	std::vector<btRigidBody*> bodies;
};

