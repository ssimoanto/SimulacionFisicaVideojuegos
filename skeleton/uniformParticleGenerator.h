#pragma once

#include "core.hpp"
#include "particle.h"
#include "particleGenerator.h"
#include <list>
#include <string>
#include "RenderUtils.hpp"
#include <random>

class UniformParticleGenerator : public ParticleGenerator {
protected:
	std::string name;
	Vector3 _pos_width;
	Vector3 _vel_width;
	Vector3 posAux;
	
	std::random_device randi;
	std::default_random_engine d;
	std::uniform_real_distribution<double> uni{ 0, 0.1 };

public:
	UniformParticleGenerator(Vector3 _posi, Vector3 _vel);
	~UniformParticleGenerator() {};
	// override para sobreescribir (tiene que heredar de la clase)
	std::list<Particle*> generateParticles() override;

};