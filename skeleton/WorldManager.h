#pragma once
#include <PxScene.h>
#include <PxPhysics.h>
#include "RenderUtils.hpp"
#include <list>

using namespace physx;

class WorldManager
{
public:
	WorldManager(physx::PxPhysics* gPhysics, physx::PxScene* gScene);
	~WorldManager();


	void addObject();
	//void addStaticObject();
	void update(double t);
	//void addForce();

	

protected:

	std::list<PxRigidDynamic*> _objects;
	RenderItem* item;
	PxPhysics* _gPhysics;
	PxScene* _gScene;

	RenderItem* rI;

	Vector3 vel = { 0, 10, -10 };
	Vector3 size = { 3, 3, 3 };

	int num;


};
