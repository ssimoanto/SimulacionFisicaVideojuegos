#include "WorldManager.h"

WorldManager::WorldManager(PxPhysics* gPhysics, PxScene* gScene)
{
	_gPhysics = gPhysics;
	_gScene = gScene;
	num = 70;
	reg = new RigidForceRegistry();
	windGen = new WindOfChangeForceGenerator({ 390,0,0 }, 0.55, 0);


}

void WorldManager::addObject()
{
	if (_objects.size() < num) {


		PxRigidDynamic* new_solid;
		new_solid = _gPhysics->createRigidDynamic(PxTransform({ 0, 0, 5 }));
		new_solid->setLinearVelocity(vel);
		new_solid->setAngularVelocity({ 0, 0, 0 });
		PxShape* shape = CreateShape(PxBoxGeometry(size));
		new_solid->attachShape(*shape);
		new_solid->setMassSpaceInertiaTensor({ size.y * size.z , size.x * size.z, size.x * size.y });
		rI = new RenderItem(shape, new_solid, { 1, 0.5, 1, 1 });
		_gScene->addActor(*new_solid);

		_objects.push_back(new_solid);

	}
}

void WorldManager::update(double t)
{
	for (auto o : _objects)
		if (windGen->isOn) {
			reg->addRegistry(windGen, o);
		}

	reg->updateForce(t);
}

void WorldManager::wind()
{
	windGen->changeWind();
}
