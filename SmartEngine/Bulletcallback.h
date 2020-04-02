#pragma once
#include <bullet/btBulletDynamicsCommon.h>
#include <iostream>
bool callbackFunc(btManifoldPoint& cp, const btCollisionObject* obj1, int id1, int index1, const btCollisionObject* obj2, int id2, int index2)
{
	std::cout << "collision" << std::endl;
	return false;
}