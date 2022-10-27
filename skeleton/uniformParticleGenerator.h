#pragma once

#include "core.hpp"
#include "particle.h"
#include "particleGenerator.h"
#include <list>
#include <string>
#include "RenderUtils.hpp"
#include <random>

class UniformParticleGenerator : public ParticleGenerator {
private:
	std::string name;
	Vector3 _pos_width;
	Vector3 _vel_width;
	std::uniform_real_distribution<double> uni{ 1.0, 2.0 };
	std::random_device randi;
	std::default_random_engine d;

public:
	UniformParticleGenerator(std::string _name, Vector3 _posi, Vector3 _vel, Vector3 d_posi, Vector3 d_vel, int _num, ParticleType t);
	~UniformParticleGenerator() {};
	// override para sobreescribir (tiene que heredar de la clase)
	std::list<Particle*> generateParticles() override;

};