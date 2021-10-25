#include "Player.h"


btTransform Player::update(float delta) {
    charCon->setGravity(delta * 2048);
    btTransform t = charCon->getGhostObject()->getWorldTransform();
    btVector3 pos = t.getOrigin();
    btQuaternion  quat = t.getRotation();
    
    camera->do_movement();

    glm::vec3 dir(0, 0, 0);
    if (charCon->onGround()) 
    {
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
            lastDir = dir;
            charCon->setWalkDirection(btVector3(dir.x, 0, dir.z).normalize() * delta * 8);
        }
        else {
            if (lastDir != glm::vec3(0)) {
                charCon->setVelocityForTimeInterval(btVector3(lastDir.x, lastDir.y, lastDir.z), 0.05);
                lastDir = glm::vec3(0);
            }
            charCon->setGravity(0);
        }
    }
    else {
            charCon->setGravity(delta * 2048);
    }

    return t;
}
