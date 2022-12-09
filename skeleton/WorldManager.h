#pragma once
#include <PxScene.h>
#include <PxPhysics.h>
#include "RenderUtils.hpp"
#include <list>
//#include "DynamicsForceRegistry.h"
//
//#include "Explosion.h"

using namespace physx;

class WorldManager
{
public:
	WorldManager(PxPhysics* gPhysics, PxScene* gScene);
	~WorldManager();


	void addDynamicObject();
	void addStaticObject();
	void update(double t);
	void addForce();

	void changeAct() {/* exPrueba->activate();*/ };

protected:

	std::list<PxRigidDynamic*> _objects;
	RenderItem* item;
	PxPhysics* gPhysics;
	PxScene* gScene;

	PxRigidDynamic* object;

	Vector3 vel = { 0, 10, -10 };
	Vector3 size = { 3, 3, 3 };


	/*DynamicsForceRegistry* dfr;
	Explosion* exPrueba;*/
};
