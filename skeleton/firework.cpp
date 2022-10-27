#include "firework.h"

Firework::Firework(Vector3 _posi, Vector3 _vel) : Particle(FIREWORKS, _posi, _vel)
{
	
}

std::list<Particle*> Firework::explode()
{
	std::list<Particle*> explosion;

	for (auto g : _gens) {
		auto f = clone();
		f->deleteGenerators();
		f->setTime(_remaining_time / 2);
		(*g).putNum(20);
		f->setMass(mass / 10);
		(*g).setParticle(f);
		//gen->setOrigin(_tr.p);
		std::list<Particle*> lp = g->generateParticles();

		for (auto p : lp) {
			//p->setPosition(/*p->getPos() + */pose.p);
			//p->setTime(1);
			//p->setVelocity(p->getVel() + v);
			//p->setMass(mass / 4);
			explosion.push_back(p);
		}
		f->setPosition({ 3877483748737483,0,0 });
	}

	return explosion;
}
