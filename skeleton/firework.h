#pragma once

#include "core.hpp"
#include "particle.h"
#include <list>
#include <memory>
#include "RenderUtils.hpp"
#include "particleGenerator.h"

//enum ParticleType { CANYON_BALL, PISTOL_BULLET };
class Firework : public Particle {
private:
	std::list<std::shared_ptr <ParticleGenerator>> _gens;

public:
	Firework(Vector3 _posi, Vector3 _vel);
	~Firework() {};
	void update(double t);
	Firework* clone() const override {
		Particle* p = new Particle(FIREWORKS, { 0,50,0 }, { 0,10,27 });
		return p;
	}
	std::list<Particle*> explode();
	inline void addGenerator(std::shared_ptr<ParticleGenerator> g) { _gens.push_back(g); }

};