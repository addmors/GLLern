#include "pEngine.h"
#include "../Sphere/Sphere.h"
#include "../mCylinder/mCylinder.h"
pEngine::pEngine() {
	collisionConfig = new btDefaultCollisionConfiguration();
	dispather = new btCollisionDispatcher(collisionConfig);
	broadPhase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispather, broadPhase, solver, collisionConfig);
	world->setGravity(btVector3(0, -10, 0));
	btTransform t;
	t.setIdentity();
	t.setOrigin({ 0,0,0 });
	btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
	btRigidBody* body = new btRigidBody(info);
	world->addRigidBody(body);
	bodies.push_back(body);
}

void pEngine::RenderSphere(btRigidBody* sphere, Sphere* sphereRen, glm::mat4 view, glm::mat4 projectoin) {
	if (sphere->getCollisionShape()->getShapeType() != SPHERE_SHAPE_PROXYTYPE)
		return;
	float r = ((btSphereShape*)sphere->getCollisionShape())->getRadius();
	btTransform t;
	sphere->getMotionState()->getWorldTransform(t);
	float mat[16];
	t.getOpenGLMatrix(mat);
	glm::mat4 glmatrix(mat[0], mat[1], mat[2], mat[3],
		mat[4], mat[5], mat[6], mat[7],
		mat[8], mat[9], mat[10], mat[11],
		mat[12], mat[13], mat[14], mat[15]);
	glmatrix = glm::scale(glmatrix, glm::vec3(r));

	sphereRen->draw(view, projectoin, glmatrix);

};

void pEngine::RenderCylinder(btRigidBody* cylinder, Cylinder* cylinderRen, glm::mat4 view, glm::mat4 projectoin) {
	if (cylinder->getCollisionShape()->getShapeType() != CYLINDER_SHAPE_PROXYTYPE)
		return;
	btVector3 exten = ((btCylinderShape*)cylinder->getCollisionShape())->getHalfExtentsWithoutMargin();
	btTransform t;
	cylinder->getMotionState()->getWorldTransform(t);
	float mat[16];
	t.getOpenGLMatrix(mat);
	glm::mat4 glmatrix(mat[0], mat[1], mat[2], mat[3],
		mat[4], mat[5], mat[6], mat[7],
		mat[8], mat[9], mat[10], mat[11],
		mat[12], mat[13], mat[14], mat[15]);
	glmatrix = glm::rotate(glmatrix, glm::radians(static_cast<GLfloat>(90)), glm::vec3(1.0, 0.0, 0.0));
	glmatrix = glm::scale(glmatrix, glm::vec3(exten.x(), exten.x(), exten.y() * 2));
	cylinderRen->SetColor({ 1.0,1.0,0 });
	cylinderRen->draw(view, projectoin, glmatrix);

};




pEngine::~pEngine() {
	for (auto& bod : bodies) {
		world->removeCollisionObject(bod);
		btMotionState* motionState = bod->getMotionState();
		btCollisionShape* shape = bod->getCollisionShape();
		delete bod;
		delete shape;
		delete motionState;
	}
	delete dispather;
	delete collisionConfig;
	delete solver;
	delete broadPhase;
	delete world;
}