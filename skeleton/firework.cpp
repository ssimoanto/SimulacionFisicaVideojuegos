#include "firework.h"

Firework::Firework(Vector3 _posi, Vector3 _vel) : Particle(FIREWORKS, _posi, _vel)
{
	poss = _posi;
}

std::list<Particle*> Firework::explode()
{
	std::list<Particle*> explosion;

	for (auto g : _gens) {
		auto f = clone();
		f->deleteGenerators();
		f->setTime(_remaining_time / 2);
		(*g).putNum(20);
		//f->setMass(mass / 10);
		(*g).setParticle(f);
		std::list<Particle*> lp = g->generateParticles();

		for (auto p : lp) {
			explosion.push_back(p);
		}
		f->setPosition({ 3877483748737483,0,0 });
	}

	return explosion;
}
