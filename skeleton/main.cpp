#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
#include "particle.h"
#include "particleSystem.h"

#include <iostream>
#include "WorldManager.h"

std::string display_text;
std::string points_text;



//int lifes = 3;

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;


PxMaterial* gMaterial = NULL;

PxPvd* gPvd = NULL;

PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
ContactReportCallback gContactReportCallback;
//Particle* p;
ParticleSystem* particleSystem;
RenderItem* suelo;
WorldManager* world;
void updateUI() {
	//lifes
	display_text = "Lifes: " + std::to_string(particleSystem->ActualLifes);
	drawText(display_text, 100, 500);
	//points
	points_text = "Points: " + std::to_string(particleSystem->points);
	drawText(points_text, 100, 200);
	
}

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	suelo = new RenderItem(CreateShape(PxBoxGeometry(500, 1, 500)), new PxTransform(-100, -5, -100), { 1,0,1,0.7 });
	particleSystem = new ParticleSystem(gScene, gPhysics);
	//particleSystem->renderPoints();
	updateUI();
	particleSystem->generateFireworkSystem();
	particleSystem->generateSpringDemo();
	particleSystem->addParticleGen();
	//particleSystem->addParticleGen(UNIFORM);
	particleSystem->getParticleGenerator("Gaussian")->changeOperative();
	/*particleSystem->getParticleGenerator("Uniform")->changeOperative();*/
	particleSystem->getParticleGenerator("CIRCLE")->changeOperative();

	particleSystem->createScene();
	
	//world = new WorldManager(gPhysics, gScene);

}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	particleSystem->update(t);
	//world->update(t);
	updateUI();
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();

	gFoundation->release();
	delete particleSystem;
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch (toupper(key))
	{
	case 'X':
		particleSystem->shootWeapon();
		break;

	case 'V':
		particleSystem->windPowerUp();
		break;
	
	case 'M': particleSystem->callStarterSpring();
		break;

	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
}


int main(int, const char* const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for (PxU32 i = 0; i < frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}