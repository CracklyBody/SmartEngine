#pragma once
#ifndef BULLETSAMPLES_H
#define BULLETSAMPLES_H

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>
#include "Model.h"


btRigidBody* addBox(float width, float height, float depth, float x, float y, float z, float mass, btDiscreteDynamicsWorld* dynamicsWorld);

unsigned int addmodel(Model* model, btDiscreteDynamicsWorld* dynamicsworld, std::vector<bulletObject*>* bodies);

btRigidBody* addSphere(float rad, float x, float y, float z, float mass, btDiscreteDynamicsWorld* dynamicsWorld);

//void renderBox(bulletObject* obj, Shader* shader, Player* player) {}
//
//void renderSphere(btRigidBody* sphere, Model* model, Shader shader, Player* player) {}

void renderPlane(btRigidBody* sphere);
#endif // !BULLETSAMPLES_H
