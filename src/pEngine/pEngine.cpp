#include "pEngine.h"
#include "../Sphere/Sphere.h"
#include "../mCylinder/mCylinder.h"
#include "../Terrian.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"



pEngine::pEngine() {
	collisionConfig = std::make_shared<btDefaultCollisionConfiguration>();
	dispather = std::make_shared<btCollisionDispatcher>(collisionConfig.get());
	broadPhase = std::make_shared<btDbvtBroadphase>();
	solver = std::make_shared<btSequentialImpulseConstraintSolver>();
	world = std::make_shared<btDiscreteDynamicsWorld>(dispather.get(), broadPhase.get(), solver.get(), collisionConfig.get());
	world->setGravity(btVector3(0, -10, 0));
}


btRigidBody* pEngine::addTerrian(Terrian* terrian) {
	btTransform t;
	t.setIdentity();
	t.setOrigin({0,0,0});
	terrian->getHeightField()->setLocalScaling({ 1,1,1 });
	std::shared_ptr<btMotionState> motion = std::make_shared<btDefaultMotionState>(t);
	btRigidBody::btRigidBodyConstructionInfo info(0.0, motion.get(), terrian->getHeightField().get());
	std::shared_ptr<btRigidBody> body = std::make_shared<btRigidBody>(info);

	world->addRigidBody(body.get());

	
	bodies.push_back(std::make_tuple(body, motion, terrian->getHeightField()));
	return body.get();
}

glm::mat4 pEngine::getmatSphere(btRigidBody* sphere) {
	if (sphere->getCollisionShape()->getShapeType() != SPHERE_SHAPE_PROXYTYPE)
		return glm::mat4();
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
	return glmatrix;
};

glm::mat4 pEngine::getmatCylinder(btRigidBody* cylinder) {
	if (cylinder->getCollisionShape()->getShapeType() != CYLINDER_SHAPE_PROXYTYPE)
		return glm::mat4();
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
	return glmatrix;
};




pEngine::~pEngine() {
	for (auto bod : bodies)
		world->removeCollisionObject(std::get<0>(bod).get());
}