#pragma once
#ifndef BULLETCALLBACK_H
#define BULLETCALLBACK_H

#include <bullet/btBulletDynamicsCommon.h>
#include <iostream>

bool callbackFunc(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2)
{
	bulletObject* ob = (bulletObject*)(obj1->getCollisionObject()->getUserPointer());
	bulletObject* ob1 = (bulletObject*)(obj2->getCollisionObject()->getUserPointer());
	if(ob!=NULL && ob1!=NULL)
		std::cout << ob->id<< " " << ob1->id<< std::endl;

	//std::cout << "collision" << std::endl;
	return false;
}
#endif // BULLETCALLBACK_H