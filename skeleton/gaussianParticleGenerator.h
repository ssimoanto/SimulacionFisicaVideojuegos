#pragma once

#include "core.hpp"
#include "particle.h"
#include "particleGenerator.h"
#include <list>
#include <string>
#include "RenderUtils.hpp"
#include <random>
using namespace std;

class GaussianParticleGenerator :public ParticleGenerator {
protected:
	std::string name;
	Vector3 std_dev_pos;
	Vector3 std_dev_vel;
	Vector3 posAux;

	normal_distribution<double>normal{ 0,1 };
	double std_dev_t;
	std::random_device randi;
	default_random_engine d;
public:
	GaussianParticleGenerator(std::string _name, Vector3 _posi,Vector3 _vel,Vector3 d_posi,Vector3 d_vel,int _num, ParticleType t);
	~GaussianParticleGenerator() {};
	std::list<Particle*> generateParticles() override;


};