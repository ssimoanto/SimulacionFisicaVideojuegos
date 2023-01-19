#pragma once

#include "core.hpp"
#include "RenderUtils.hpp"
#include "particle.h"
#include "firework.h"
#include "particleGenerator.h"
#include <list>
#include <vector>
#include "forceGenerator.h"
#include "particleForceRegistry.h"
using namespace physx;
enum GeneratorName {
	GAUSSIAN, UNIFORM, FIREWORK, SHOOT_CANYON
};
class ParticleSystem {
protected:
	std::list<Particle*> _particles;
	std::list<std::shared_ptr<ParticleGenerator>>_particle_generators;
	std::vector<Firework*> _firework_pool;
	Particle* particle;
	

	ParticleForceRegistry* pFR;
	RigidForceRegistry* rFR;
	physx::PxScene* scene;
	physx::PxPhysics* physics;
	PxRigidStatic* floor;
	RenderItem* floorItem;
	std::string display_text;
	std::string points_text;
	AnchoredSpringFG* f3;
	Particle* p3;

public:
	
	WindOfChangeForceGenerator* windGen;
	WhirlwindOfChangeForceGenerator* whirl;
	

	Firework* _firework = nullptr;
	bool isOn = false;
	ParticleSystem(physx::PxScene* Scene, physx::PxPhysics* Physics);
	~ParticleSystem() {
		deleteAllFruits();
		deleteStatics();
		
		for (auto e : _particles)
		{
			delete e;
		}
		_particles.clear();
		for (auto e : _firework_pool) {
			delete e;
		}
		_firework_pool.clear();
		while (!_particle_generators.empty())
			_particle_generators.pop_front();

		floor=nullptr;
		delete pFR;
		delete rFR;
		
	};
	void addParticleGen();
	void update(double t);
	std::shared_ptr<ParticleGenerator> getParticleGenerator(std::string gn);
	void onParticleDeath(Particle*);
	void generateFireworkSystem();
	bool isFireworkAlive();
	
	void shootFirework(int type);
	
	//muelles
	void generateSpringDemo();
	/*void slinky();
	void buoyancy();*/

	//proyecto final
	void createScene();
	void createLevels();
	void startNextLevel();
	void callStarterSpring();
	struct Level {
		int numOfFruits;
		int frecuency;
		int fruitsGeneratedInLevel;
	};
	Level* level1;
	Level* level2;
	Level* level3;
	Level* actualLevel=nullptr;


	//lose
	bool hasLost = false;
	//win
	bool hasWon = false;
	
	void victoryFireworks();
	
	bool isPlayerOnLevel = false;
	bool whirlOn = false;
	int whirlTimer = 200;
	int bombTimer = 150;
	bool bombOn = false;
	bool windOn = false;
	int windTimer = 1000;
	int ActualLifes = 3;
	int points = 0;
	int lastShoot = 0;
	struct DynamicBody {
		physx::PxRigidActor* body;
		RenderItem* rendIt;
		
		bool _alive = false;
		bool isBomb = false;

		float lifeTime = 1.0;

		bool isWind = false;
		
	};
	struct StaticBody {
		physx::PxRigidActor* body;
		RenderItem* rendIt;
	};
	
	std::list<StaticBody*>statics;
	std::list<DynamicBody*>fruits;
	std::list<DynamicBody*>weapons;
	
	bool canShoot = true;

	void shootWeapon();
	void addFruit(double t);
	void deleteDynamicBody(DynamicBody* d,std::string s);
	//ui
	void renderLifes();
	void renderPoints();
	void stopLoseFeedback();
	
	//colissions
	void collisionsBetweenFruitsAndFloor(double t);
	void collisionsBetweenFruitsAndWeapons(double t);

	//delete 
	void deleteAllFruits();
	void deleteStatics();


	void loseLevel();
	void resetLoseParams();

	//wind power up
	void windPowerUp();
};