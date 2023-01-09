#include "firework.h"

Firework::Firework(Vector3 _posi, Vector3 _vel) : Particle(FIREWORKS, _posi, _vel)
{
	pose.p = _posi;

}

std::list<Particle*> Firework::explode()
{
	/*
	if (getNumGen() + 1 <  4) {
				auto pt = clone();
				pt->setNumGen(numGen + 1);
				pt->setLifeTime(2.0);
				auto r = rand() % 255 + 0;
				auto g = rand() % 255 + 0;
				auto b = rand() % 255 + 0;
				pt->setColor(Vector4(r / 255.0, g / 255.0, b / 255.0, 1.0));
				pt->setAcceleration(Vector3(0, -10 / (numGen + 1), 0));
				pt->setVelocity(pt->getVelocity());


				(*it).setNumPart(10);
				(*it).setParticle(pt);
				(*it).generateParticles(list);
				pt->setPosition(Vector3(1000000, 100000000, 100000));
			}


	*/
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
