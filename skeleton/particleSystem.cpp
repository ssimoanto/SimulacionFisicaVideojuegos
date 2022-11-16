#include "particleSystem.h"
#include "gaussianParticleGenerator.h"
#include "uniformParticleGenerator.h"

ParticleSystem::ParticleSystem()
{
	pFR = new ParticleForceRegistry();
	gravGen = new GravityForceGenerator({ 0,-9.8,0 });
	windGen = new WindOfChangeForceGenerator({ 7,0,0 }, 0.55, 0);
	windGen->isOn = true;
}

void ParticleSystem::addParticleGen(/*GeneratorName gn*/)
{
	_particle_generators.push_back(std::shared_ptr<ParticleGenerator>(new GaussianParticleGenerator("Gaussian", { 0, 50, 0 }, { 0,10,27 }, { 1,1,2.5 }, { 1,1,1 }, 1, GAUSSIAN_BALL)));
	_particle_generators.push_back(std::shared_ptr<ParticleGenerator>(new UniformParticleGenerator({ 0, 50, 0 }, { 0, 0, 0 })));
	//_particle_generators.push_back(std::shared_ptr<ParticleGenerator>(new UniformParticleGenerator("Uniform", { 0, 50, 0 }, { 0,10,27 }, 12, GAUSSIAN_BALL)));
	/*switch (gn)
	{
	case GAUSSIAN:

		break;
	case UNIFORM:
		break;
	case FIREWORK:
		_firework = new Firework({ 0,50,0 }, { 0,10,27 });
		break;
	default:
		break;
	}*/
}

void ParticleSystem::update(double t)
{
	pFR->updateForce(t);
	//act generadores
	for (auto gen : _particle_generators) {
		if (gen->isOperative()) {
			std::list<Particle*> lista = gen->generateParticles();
			for (auto partic : lista) {
				_particles.push_back(partic);
			}
		}
	}
	//act particulas
	auto it = _particles.begin();
	while (it != _particles.end()) {
		if ((*it)->particleExists()) {
			(*it)->update(t);
			if (gravGen->isOn) pFR->addRegistry(gravGen, *it);
			if (windGen->isOn) pFR->addRegistry(windGen, *it);
			++it;
		}
		else {
			pFR->deleteParticleRegistry(*it);
			onParticleDeath(*it);
			
			delete* it;
			it = _particles.erase(it);
		}
	}

}

std::shared_ptr<ParticleGenerator> ParticleSystem::getParticleGenerator(std::string gn)
{
	for (auto e : _particle_generators)
	{
		if ((*e).getGeneratorName() == gn) return e;
	}
	return nullptr;
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
	std::shared_ptr<ParticleGenerator>gF(new GaussianParticleGenerator("Firework", { 0, 0, 0 }, { 0,20,0 }, { 0,0,0 }, { 10,10,10 }, 4, FIREWORKS));
	auto f = new Firework(Vector3(1000000000, 0, 0), Vector3(0, 60, 0));
	f->addGenerator(gF);
	f->setTime(1.5);
	_firework_pool.push_back(f);
	gF->changeOperative();
	_particle_generators.push_back(gF);
	//
	//std::shared_ptr<ParticleGenerator>gF(new GaussianParticleGenerator("Firework", { 0, 0, 0 }, { 0,20,0 }, { 0,0,0 }, { 10,10,10 }, 4, FIREWORKS));
	/*f = new Firework(Vector3(1000000000, 0, 0), Vector3(0, 60, 0));
	f->addGenerator(gF);
	f->setTime(1.5);
	_firework_pool.push_back(f);
	gF->changeOperative();
	_particle_generators.push_back(gF);*/

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
