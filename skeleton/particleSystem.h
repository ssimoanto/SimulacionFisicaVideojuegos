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
enum GeneratorName {
	GAUSSIAN, UNIFORM, FIREWORK, SHOOT_CANYON
};
class ParticleSystem {
protected:
	std::list<Particle*> _particles;
	std::list<std::shared_ptr<ParticleGenerator>>_particle_generators;
	std::vector<Firework*> _firework_pool;

	Particle* particle;
	ParticleGenerator* _firework_gen;
	GravityForceGenerator* gravGen;
	WindOfChangeForceGenerator* windGen;
	WhirlwindOfChangeForceGenerator* whirl;
	ExplosionBoomForceGenerator* boom;
	ParticleForceRegistry* pFR;
public:

	Firework* _firework = nullptr;
	bool isOn = false;
	ParticleSystem();
	~ParticleSystem() {

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
	void activeGrav() { gravGen->isOn = true; }
	void boomNow() {
		for (auto p : _particles) {
			boom->tiempo = GetLastTime();
			pFR->addRegistry(boom, p);
		}
	}
};