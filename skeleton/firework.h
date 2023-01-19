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
	int numDivisions = 0;
	Firework(Vector3 _posi, Vector3 _vel);
	~Firework() {};
	void update(double t);
	Firework* clone() const override {
		Firework* p = new Firework(pose.p, v);
		p->setTime(_remaining_time); p->numDivisions = numDivisions;
		for (auto g : _gens) p->addGenerator(g);
		return p;
	}
	std::list<Particle*> explode();
	inline void addGenerator(std::shared_ptr<ParticleGenerator> g) { _gens.push_back(g); }
	void deleteGenerators() {
		while (!_gens.empty())
			_gens.pop_front();
	}
};