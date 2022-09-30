#include "shoot.h"

Shoot::Shoot(Vector3 pos, double vel, Vector3 ac, double damping)
{
	p = pos;
	dir = GetCamera()->getDir();
	v = vel * dir;
	a = ac;
	d = damping;}

void Shoot::create() {

	particle = new Particle(p, v, a, d);
	vec.push_back(particle);
}

//void Shoot::integrate(double t)
//{
//	// Trivial  case, infinite mass --> do nothing
//	//if (inverse_mass <=  0.0f) return;
//	// Update position
//	pose.p += v * t;
//	// Update linear  velocity
//	v += a * t;
//	// Impose drag (damping)
//	v *= powf(d,  t);
//}


