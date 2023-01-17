#include "firework.h"

Firework::Firework(Vector3 _posi, Vector3 _vel) : Particle(FIREWORKS, _posi, _vel)
{
	pose.p = _posi;

}

std::list<Particle*> Firework::explode()
{
	
	std::list<Particle*> explosion;

	for (auto g : _gens) {
		if (numDivisions + 1 < 3) {
			auto f = clone();
			
			f->numDivisions = numDivisions + 1;
			//f->deleteGenerators();
			f->setTime(2);
			(*g).putNum(5);
			//f->setMass(mass / 10);
			(*g).setParticle(f);
			f->a = (Vector3(0, -10 / 2, 0));
			f->setVelocity(f->getVel());
			std::list<Particle*> lp = g->generateParticles();

			for (auto p : lp) {
				explosion.push_back(p);
			}
			f->setPosition({ 387748,0,0 });
		}

	}

	return explosion;
}
