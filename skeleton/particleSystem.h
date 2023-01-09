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
class GameManager {
protected:
	std::list<Particle*> _particles;
	std::list<std::shared_ptr<ParticleGenerator>>_particle_generators;
	std::vector<Firework*> _firework_pool;

	Particle* particle;
	ParticleGenerator* _firework_gen;

	ParticleForceRegistry* pFR;
	physx::PxScene* scene;
	physx::PxPhysics* physics;
	PxRigidStatic* floor;
	RenderItem* floorItem;
	std::string display_text;
	std::string points_text;

public:
	GravityForceGenerator* gravGen;
	WindOfChangeForceGenerator* windGen;
	WhirlwindOfChangeForceGenerator* whirl;
	ExplosionBoomForceGenerator* boom;

	Firework* _firework = nullptr;
	bool isOn = false;
	GameManager(physx::PxScene* Scene, physx::PxPhysics* Physics);
	~GameManager() {

		for (auto e : _particles)
		{
			delete e;
		}
		_particles.clear();
		for (auto e : _firework_pool) {
			delete e;
		}
		_firework_pool.clear();
		for (auto e : _particle_generators)
		{
			_particle_generators.pop_front();
		}
		delete gravGen;
		delete windGen;
	};
	void addParticleGen(/*GeneratorName gn*/);
	void update(double t);
	std::shared_ptr<ParticleGenerator> getParticleGenerator(std::string gn);
	void onParticleDeath(Particle*);
	void generateFireworkSystem();
	bool isFireworkAlive();
	//void create();
	void shootFirework(int type);
	void activeGrav() { gravGen->isOn = !gravGen->isOn; }
	void boomNow() {
		for (auto p : _particles) {
			boom->tiempo = GetLastTime();
			pFR->addRegistry(boom, p);
		}
	}
	//muelles
	void generateSpringDemo();
	void slinky();
	void buoyancy();

	//proyecto final
	void createScene();
	int ActualLifes = 3;
	int points = 0;
	int lastShoot = 0;
	struct DynamicBody {
		physx::PxRigidActor* body;
		RenderItem* rendIt;
		
		//double _remaining_time = 5;
		bool _alive = false;

		float lifeTime = 1.0;
		/*vector<string> forcesNames;*/
		/*double maxTimeAlive;
		double timeAlive = -1.0;*/
	};
	struct StaticBody {
		physx::PxRigidActor* body;
		RenderItem* rendIt;
	};
	//struct Weapon {
	//	physx::PxRigidActor* body;
	//	RenderItem* rendIt;
	//	double _remaining_time = 5;
	//	bool _alive = false;
	//	/*vector<string> forcesNames;*/
	//	/*double maxTimeAlive;
	//	double timeAlive = -1.0;*/
	//};

	std::list<DynamicBody*>fruits;
	std::list<DynamicBody*>weapons;
	DynamicBody* bull;
	bool canShoot = true;

	void shootWeapon();
	void addFruit(double t);
	void deleteDynamicBody(DynamicBody* d,std::string s);
	//ui
	void renderLifes();
	void renderPoints();
	Particle* pLife;
	//colissions
	void collisionsUpdate(double t);
};