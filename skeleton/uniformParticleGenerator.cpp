#include "uniformParticleGenerator.h"

UniformParticleGenerator::UniformParticleGenerator(Vector3 _posi, Vector3 _vel, Vector3 d_posi, Vector3 d_vel, int _num) : ParticleGenerator("GaussGen", _posi, _vel)
{

}

std::list<Particle*> UniformParticleGenerator::generateParticles()
{
	return std::list<Particle*>();
}

