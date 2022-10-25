#pragma once

#include "core.hpp"
#include "particle.h"
#include <list>
#include <string>
#include "RenderUtils.hpp"

class ParticleGenerator {
protected:
	std::string _name;

	Vector3 _mean_pos;
	Vector3 _mean_vel;

	double _generation_probability;

	int _num_particles;

	Particle* _model;
public:
	ParticleGenerator(std::string _name,Vector3 _posi,Vector3 _vel);
	~ParticleGenerator() {};
	void setParticle(Particle *model);
	// = 0 -> virtual puro, hay que sobreescribirlo si o si
	virtual std::list<Particle*> generateParticles() = 0 {}

};