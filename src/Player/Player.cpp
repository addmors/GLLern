#include "Player.h"

void Player::show()
{
	if (currentShader == nullptr) return;
	if (Persone == nullptr) return;
	Persone->DrawAnim(*currentShader);
}
void Player::update() {
    btTransform t;
    t = charCon->getGhostObject()->getWorldTransform();
    btVector3 pos = t.getOrigin();
    btQuaternion  quat = t.getRotation();
    
    glm::vec3 dir(0, 0, 0);
    if (keys->at(87))
        dir += camera->cameraFront;

    if (keys->at(83))
        dir -= camera->cameraFront;
    if (keys->at(65))
        dir -= glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp));
    if (keys->at(68))
        dir += glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp));
    if (dir != glm::vec3(0, 0, 0))
    {
        if (charCon->onGround())
            charCon->setWalkDirection(btVector3(dir.x, 0, dir.z).normalized() / 10);
        else
            charCon->setWalkDirection(btVector3(dir.x, 0, dir.z).normalized() / 30);
    }

    else
        charCon->setWalkDirection(btVector3(0, -10, 0));

    camera->objPos = glm::vec3(pos.x(), pos.y(), pos.z());
    
}
