#pragma once
#include "../Model/Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include"btBulletDynamicsCommon.h"
#include"BulletCollision/CollisionDispatch/btGhostObject.h"
#include"BulletDynamics/Character/btKinematicCharacterController.h"
#include <glm/gtc/type_ptr.hpp>
#include"../Camera/Camera.h"
#include "../pEngine/pEngine.h"
#include<string>


#define MAX_SPEED 2

class Shader;

class Player
{	
public:
	pEngine* pengine;
	vector<bool>* keys;

	Player(Model& model, Camera& _camera, Shader& _curshader, pEngine* _pengine) 
		:pengine(_pengine),
		Persone(&model),
		camera(&_camera),
		currentShader(&_curshader)
	{

		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(0, 0, 0));

		btConvexShape* capsule = new btCapsuleShape(0.5, 0.5);

		m_ghostObject = new btPairCachingGhostObject();
		m_ghostObject->setWorldTransform(startTransform);
		m_ghostObject->setCollisionShape(capsule);
		m_ghostObject->setFriction(20);
		m_ghostObject->setRollingFriction(20);

		m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		
		charCon = new btKinematicCharacterController(m_ghostObject, capsule, 0.05f);
		charCon->setGravity(btScalar(10));
		charCon->setMaxJumpHeight(1.5);
		
		pengine->getBroadPhase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
		pengine->getWorld()->addCollisionObject(m_ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter);
		pengine->getWorld()->addAction(charCon);
		
	};

	void destroy() {
		pengine->getWorld()->removeCollisionObject(m_ghostObject);
		if (m_ghostObject != nullptr) delete m_ghostObject;
		if (charCon != nullptr) delete charCon;
	};
	void update();
	void show();
	
	Model* Persone = nullptr;
	Camera* camera = nullptr;
	Shader* currentShader = nullptr;
	string name = "def_name";
	bool ongroud = 0, iscollidet  = 0;
	btPairCachingGhostObject* m_ghostObject;
	btKinematicCharacterController* charCon;
};

