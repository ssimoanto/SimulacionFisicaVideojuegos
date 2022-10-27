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
		_particle_generators.push_back(std::shared_ptr<ParticleGenerator>(new GaussianParticleGenerator({ 0,50,0 }, { 0,10,27 }, { 1,1,2.5 }, { 1,1,1 }, 1, GAUSSIAN_BALL)));
		break;
	case UNIFORM:
		break;
	case FIREWORK:
		_firework = new Firework({ 0,50,0 }, { 0,10,27 });
		break;
	default:
		break;
	}
}

void ParticleSystem::update(double t)
{
	//act generadores
	for (auto gen : _particle_generators) {
		if (gen->isOperative()) {
			std::list<Particle*> lista = gen->generateParticles();
			for (auto partic : lista) {
				_particles.push_back(partic);
			}
		}

	}
	//act particulass
	//for (auto p : _particles) p->update(t);

	//borrar particulas
	auto it = _particles.begin();
	while (it != _particles.end()) {
		if ((*it)->particleExists()) {
			(*it)->update(t);
			++it;
		}
		else {
			onParticleDeath(*it);
			delete* it;
			it = _particles.erase(it);
		}
	}
}

ParticleGenerator* ParticleSystem::getParticleGenerator(std::string name)
{
	return _firework_gen;
}

void ParticleSystem::onParticleDeath(Particle* p)
{
	Firework* fk = dynamic_cast<Firework*>(p);
	if (fk != nullptr) {
		auto part = fk->explode();
		for (auto p : part)
			_particles.push_back(p);
		part.clear();
	}
}

void ParticleSystem::generateFireworkSystem()
{
	std::shared_ptr<ParticleGenerator>gF(new GaussianParticleGenerator({ 0,0,0 }, { 0,20,0 }, { 0,0,0 }, { 10,10,10 }, 4, FIREWORKS));
	auto f = new Firework(Vector3(1000000000, 0, 0), Vector3(0, 60, 0));
	f->addGenerator(gF);
	f->setTime(1.5);
	_firework_pool.push_back(f);
	gF->changeOperative();
	_particle_generators.push_back(gF);
}

bool ParticleSystem::isFireworkAlive()
{
	return(_firework != nullptr);
}

//void ParticleSystem::create() {
//
//	/*particle = new Particle(PISTOL_BULLET, pose.p, v);
//	_particles.push_back(particle);*/
//}

void ParticleSystem::shootFirework(int type)
{
	auto f = _firework_pool[type]->clone();
	f->setPosition({ 0,0,0 });
	_particles.push_back(f);
}
