#pragma once

#include "core.hpp"
#include "particle.h"
#include "particleGenerator.h"
#include <list>
#include <string>
#include "RenderUtils.hpp"

class UniformParticleGenerator : ParticleGenerator {
private:
	Vector3 _pos_width;
	Vector3 _vel_width;

public:
	UniformParticleGenerator(Vector3 _posi, Vector3 _vel, Vector3 d_posi, Vector3 d_vel, int _num);
	~UniformParticleGenerator() {};
	// override para sobreescribir (tiene que heredar de la clase)
	std::list<Particle*> generateParticles() override;

};