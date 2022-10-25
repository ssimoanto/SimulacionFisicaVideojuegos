#include "particleSystem.h"
#include "gaussianParticleGenerator.h"

ParticleSystem::ParticleSystem()
{

}

void ParticleSystem::addParticleGen(GeneratorName gn)
{
	switch (gn)
	{
	case GAUSSIAN:
		_particle_generators.push_back(new GaussianParticleGenerator({ 0,50,0 }, { 0,10,27 }, { 1,1,1 }, { 1,1,1 }, 10));
		break;
	case UNIFORM:
		break;
	default:
		break;
	}
}

void ParticleSystem::update(double t)
{
	//act generadores
	for (auto gen : _particle_generators) {
		auto lista = gen->generateParticles();
		for (auto partic : lista) {
			_particles.push_back(partic);
		}
	}
	//act particulass
	for (auto p : _particles) p->update(t);

	//borrar particulas
	auto it = _particles.begin();
	while (it != _particles.end()) {
		if ((*it)->particleExists()) {
			(*it)->update(t);
			++it;
		}
		else {


			delete* it;
			it = _particles.erase(it);
		}

	}
}

ParticleGenerator* ParticleSystem::getParticleGenerator(std::string name)
{

	return nullptr;
}

void ParticleSystem::onParticleDeath(Particle*)
{
}

void ParticleSystem::generateFireworkSystem()
{
}

void ParticleSystem::create() {

	particle = new Particle(PISTOL_BULLET);
	_particles.push_back(particle);
}

void ParticleSystem::shootFirework(int type)
{
}
