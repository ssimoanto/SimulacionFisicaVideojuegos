#include "particleSystem.h"
#include "gaussianParticleGenerator.h"
#include "uniformParticleGenerator.h"

ParticleSystem::ParticleSystem()
{
	pFR = new ParticleForceRegistry();
	gravGen = new GravityForceGenerator({ 0,-9.8,0 });
	windGen = new WindOfChangeForceGenerator({ 20,0,0 }, 0.55, 0);
	//windGen->isOn = true;
	whirl = new WhirlwindOfChangeForceGenerator(7, { 0,0,0 }, { 6,2,0 });
	//whirl->isOn = true;

	boom = new ExplosionBoomForceGenerator(200, 5, { 0,0,0 }, 2);
	//boom->isOn = true;
}

void ParticleSystem::addParticleGen(/*GeneratorName gn*/)
{
	_particle_generators.push_back(std::shared_ptr<ParticleGenerator>(new GaussianParticleGenerator("Gaussian", { 0, 0, 0 }, { -16,150,-27 }, { 15,1,15 }, { 1,1,1 }, 1, GAUSSIAN_BALL)));
	_particle_generators.push_back(std::shared_ptr<ParticleGenerator>(new UniformParticleGenerator({ 0, 0, 0 }, { 0, 0, -100 })));
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
			if (whirl->isOn) pFR->addRegistry(whirl, *it);


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
void ParticleSystem::shootFirework(int type)
{
	auto f = _firework_pool[type]->clone();
	f->setPosition({ 0,0,0 });
	_particles.push_back(f);
}
void ParticleSystem::generateSpringDemo()
{
	//Particle* p1=new Particle()
	Particle* p1 = new Particle(MUELLE, { -10,10,0 }, { 0,0,0 });
	Particle* p2 = new Particle(MUELLE, { 10,10,0 }, { 0,0,0 });

	auto f1 = new SpringForceGenerator(10, 10, p2);

	auto f2 = new SpringForceGenerator(10, 10, p1);

	pFR->addRegistry(f1, p1);
	pFR->addRegistry(f2, p2);

	_particles.push_back(p1);
	_particles.push_back(p2);


	Particle* p3 = new Particle(MUELLE, { 0,30,0 }, { 0,0,0 });


	auto f3 = new AnchoredSpringFG(20, 10, { 0.0,50.0,0.0 });

	pFR->addRegistry(f3, p3);
	_particles.push_back(p3);
}

void ParticleSystem::slinky()
{
	float y = 60.0f;
	//Particle* pAnt;

	Particle* p = new Particle(MUELLE, { 0,y,0 }, { 0,0,0 });
	_particles.push_back(p);
	y -= 5;
	Particle* p2 = new Particle(MUELLE, { 0,y,0 }, { 0,0,0 });
	_particles.push_back(p2);
	pFR->addRegistry(gravGen, p2);
	y -= 5;
	Particle* p3 = new Particle(MUELLE, { 0,y,0 }, { 0,0,0 });
	_particles.push_back(p3);
	pFR->addRegistry(gravGen, p3);
	y -= 5;
	Particle* p4 = new Particle(MUELLE, { 0,y,0 }, { 0,0,0 });
	_particles.push_back(p4);
	pFR->addRegistry(gravGen, p4);
	y -= 5;
	Particle* p5 = new Particle(MUELLE, { 0,y,0 }, { 0,0,0 });
	_particles.push_back(p5);
	pFR->addRegistry(gravGen, p5);
	y -= 5;
	Particle* p6 = new Particle(MUELLE, { 0,y,0 }, { 0,0,0 });
	_particles.push_back(p6);
	pFR->addRegistry(gravGen, p6);

	auto f = new SpringForceGenerator(10, 1, p);
	pFR->addRegistry(f, p2);
	auto f1 = new SpringForceGenerator(10, 1, p2);
	pFR->addRegistry(f1, p3);
	auto f2 = new SpringForceGenerator(20, 1, p3);
	pFR->addRegistry(f2, p4);
	auto f3 = new SpringForceGenerator(30, 1, p4);
	pFR->addRegistry(f3, p5);
	auto f4 = new SpringForceGenerator(40, 1, p5);
	pFR->addRegistry(f3, p6);

}

void ParticleSystem::buoyancy()
{
	Particle* p = new Particle(BOX_PART, { 0,21,0 }, { 0,0,0 });
	p->setMass(50.0);
	_particles.push_back(p);

	auto f = new BuoyancyForceGenerator(12, 7, 1000);
	pFR->addRegistry(gravGen, p);

	pFR->addRegistry(f, p);
}

