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

	bool _operative;
public:
	ParticleGenerator(std::string _name,Vector3 _posi,Vector3 _vel);
	~ParticleGenerator() {};
	void setParticle(Particle *model);
	// = 0 -> virtual puro, hay que sobreescribirlo si o si
	virtual std::list<Particle*> generateParticles() = 0;
	inline void setMeanPos(Vector3 p) { _mean_pos = p; }
	inline bool isOperative() { return _operative; }
	inline void changeOperative() { _operative = !_operative; }
	inline void putNum(int n) { _num_particles = n; }

};