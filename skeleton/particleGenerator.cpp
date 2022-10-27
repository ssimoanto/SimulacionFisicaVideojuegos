#include "particleGenerator.h"
#include "gaussianParticleGenerator.h"

ParticleGenerator::ParticleGenerator(std::string _name, Vector3 _posi, Vector3 _vel)
{
	name = _name;
}

void ParticleGenerator::setParticle(Particle* model)
{
	_model = model;
}

std::list<Particle*> generateParticles()
{
	return std::list<Particle*>();
}

std::string ParticleGenerator::getGeneratorName()
{
	return name;
}
