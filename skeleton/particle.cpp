#include "particle.h"

Particle::Particle(Vector3 Pos, Vector3 Vel)
{
	v = Vel;
	pose = physx::PxTransform(Pos.x, Pos.y, Pos.z);
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1.0)), &pose, { 0,0.5,0.5,0.3 });
}

void Particle::integrate(double t)
{
	// Trivial  case, infinite mass --> do nothing
	//if (inverse_mass <=  0.0f) return;
	// Update position
	pose.p += v * t;
	// Update linear  velocity
	//v += a * t;
	// Impose drag (damping)
	//v *= powf(damping,  t);}
}
