#pragma once

#include "core.hpp"
#include "RenderUtils.hpp"
#include "particle.h"
#include "firework.h"
#include "particleGenerator.h"
#include <list>
#include <vector>
enum GeneratorName {
	GAUSSIAN, UNIFORM
};
class ParticleSystem {
protected:
	std::list<Particle*> _particles;
	std::list<ParticleGenerator*> _particle_generators;
	std::vector<Firework*> _firework_pool;

	Particle* particle;
	ParticleGenerator* _firework_gen;
	public:
		ParticleSystem();
		~ParticleSystem() {
			
			for ( auto e: _particles)
			{
				delete e;
			}
			_particles.clear();
		};
		void addParticleGen(GeneratorName gn);
		void update(double t);
		ParticleGenerator* getParticleGenerator(std::string name);
		void onParticleDeath(Particle*);
		void generateFireworkSystem();

		void create();
		void shootFirework(int type);
};