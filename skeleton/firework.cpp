#include "firework.h"

Firework::Firework(Vector3 _posi, Vector3 _vel) : Particle(FIREWORKS, _posi, _vel)
{
	
}

std::list<Particle*> Firework::explode()
{
	std::list<Particle*> explosion;

	for (auto g : _gens) {
		gen->setOrigin(_tr.p);
		std::list<Particle*> lp = g->generateParticles();

		for (Particle* p : lp) {
			p->setVelocity(p->getVel() + v);
			explosion.push_back(p);
		}
	}

	return explosion;
}
