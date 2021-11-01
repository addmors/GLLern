#include "pEngine.h"
#include "../Sphere/Sphere.h"
#include "../mCylinder/mCylinder.h"
#include "../Terrian.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"




pEngine::pEngine() {
	collisionConfig = std::make_shared<btDefaultCollisionConfiguration>();
	dispather = std::make_shared<btCollisionDispatcher>(collisionConfig.get());
	btGImpactCollisionAlgorithm::registerAlgorithm(dispather.get());
	broadPhase = std::make_shared<btDbvtBroadphase>();
	solver = std::make_shared<btSequentialImpulseConstraintSolver>();
	world = std::make_shared<btDiscreteDynamicsWorld>(dispather.get(), broadPhase.get(), solver.get(), collisionConfig.get());
	world->setGravity(btVector3(0, -20, 0));
}


btRigidBody* pEngine::addTerrian(Terrian* terrian) {
	btTransform t;
	t.setIdentity();
	t.setOrigin({0,0,0});
	std::shared_ptr<btMotionState> motion = std::make_shared<btDefaultMotionState>(t);
	btRigidBody::btRigidBodyConstructionInfo info(0.0, motion.get(), terrian->getHeightField().get());
	std::shared_ptr<btRigidBody> body = std::make_shared<btRigidBody>(info);
	body->setFriction(10);
	this->sizePlane = terrian->getSize()/2;
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
}


glm::mat4 pEngine::getmatBox(btRigidBody* box)
{
	if (box->getCollisionShape()->getShapeType() != BOX_SHAPE_PROXYTYPE)
		return glm::mat4();

	btVector3 exten = ((btBoxShape*)box->getCollisionShape())->getHalfExtentsWithoutMargin();
	btTransform t;
	box->getMotionState()->getWorldTransform(t);

	float mat[16];
	t.getOpenGLMatrix(mat);
	glm::mat4 glmatrix(mat[0], mat[1], mat[2], mat[3],
		mat[4], mat[5], mat[6], mat[7],
		mat[8], mat[9], mat[10], mat[11],
		mat[12], mat[13], mat[14], mat[15]);
	//glmatrix = glm::rotate(glmatrix, glm::radians(static_cast<GLfloat>(90)), glm::vec3(1.0, 0.0, 0.0));
	glmatrix = glm::scale(glmatrix, glm::vec3(exten.x(), exten.y(), exten.z()));
	return glmatrix;
}

std::vector<size_t> pEngine::genMatrices(std::map<std::string, std::vector<glm::mat4>::iterator>& Instance)
{
	vector<size_t> gener(Instance.size());
	for (auto [a, b, c] : bodies) {
		if (a->getInvMass() != 0) {

			if (a->getCollisionShape()->getShapeType() == CYLINDER_SHAPE_PROXYTYPE) {
				*Instance.at("cylinder") = getmatCylinder(a.get());
				Instance.at("cylinder")++;
				gener.at(0)++;
			}
			else if (a->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE) {
				*Instance.at("sphere") = getmatSphere(a.get());
				Instance.at("sphere")++;
				gener.at(1)++;
			}
			else if (a->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE) {
				*Instance.at("box") = getmatBox(a.get());
				 Instance.at("box")++;
				 gener.at(2)++;
			}
		}
	}
	return gener;
}
;



pEngine::~pEngine() {
	for (auto bod : bodies)
		world->removeCollisionObject(std::get<0>(bod).get());
}


void pEngine::step(float delta) {
	{
		if (world != nullptr)  world->stepSimulation(delta);
		auto curr = bodies.begin();
		while (curr != bodies.end()) {
			auto nextBody = std::next(curr);
			btVector3 t = std::get<0>(*curr)->getWorldTransform().getOrigin();
			if (abs(t.x()) > sizePlane || abs(t.z()) > sizePlane) {
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
}


btRigidBody* pEngine::addSphere(float rad, float x, float y, float z, float mass) {
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

	body->setFriction(10);

	world->addRigidBody(body.get());
	bodies.push_back(std::make_tuple(body, motion, sphere));

	return body.get();
}


btRigidBody* pEngine::addCylinder(float d, float h, float x, float y, float z, float mass) {
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
	body->setFriction(10);

	world->addRigidBody(body.get());
	bodies.push_back(std::make_tuple(body, motion, cyrcle));

	return body.get();
}

btRigidBody* pEngine::addBox(float height, float widht, float length, float x, float y, float z, float mass) {
	btTransform t;
	t.setIdentity();
	t.setOrigin({ x,y,z });
	btVector3 inertial;
	std::shared_ptr<btBoxShape> box = std::make_shared<btBoxShape>(btVector3(widht / 2.0, height / 2.0, length / 2.0));
	box->setMargin(0);
	if (mass != 0) {
		box->calculateLocalInertia(mass, inertial);
	}
	std::shared_ptr<btMotionState> motion = std::make_shared<btDefaultMotionState>(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion.get(), box.get(), inertial);

	std::shared_ptr<btRigidBody> body = std::make_shared<btRigidBody>(info);

	body->setFriction(10);

	world->addRigidBody(body.get());
	bodies.push_back(std::make_tuple(body, motion, box));

	return body.get();
}
