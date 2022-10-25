#include "particleGenerator.h"

ParticleGenerator::ParticleGenerator(std::string _name, Vector3 _posi, Vector3 _vel)
{

}

void ParticleGenerator::setParticle(Particle* model)
{
	_model = model;
}

std::list<Particle*> generateParticles()
{
	return std::list<Particle*>();
}
