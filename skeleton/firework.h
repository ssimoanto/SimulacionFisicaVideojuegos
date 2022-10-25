#pragma once

#include "core.hpp"
#include "particle.h"
#include <list>
#include <memory>
#include "RenderUtils.hpp"

//enum ParticleType { CANYON_BALL, PISTOL_BULLET };
class Firework : Particle {
private:
	std::list<std::shared_ptr <Particle>> _gens;

public:
	Firework();
	~Firework() {};
	void update(double t);
	virtual Particle* clone() const {
		Particle* p = new Particle(FIREWORKS);
		return p;
	}
	std::list<Particle*> explode();

};