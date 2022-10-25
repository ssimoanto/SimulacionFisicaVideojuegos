#include "particle.h"

Particle::Particle(ParticleType type)
{
	// Vector3 Pos, Vector3 Vel, Vector3 Ac, double damping
	_type = type;
	switch (_type) {
	case CANYON_BALL:
		v = 200 * GetCamera()->getDir();
		a = physx::PxVec3(0, -9.8, 0);
		pose = physx::PxTransform(GetCamera()->getEye().x, GetCamera()->getEye().y, GetCamera()->getEye().z);
		mass = 17.0f;
		break;
	case PISTOL_BULLET:
		v = 400 * GetCamera()->getDir();
		a = physx::PxVec3(0, -9.8, 0);
		pose = physx::PxTransform(GetCamera()->getEye().x, GetCamera()->getEye().y, GetCamera()->getEye().z);
		mass = 10.0f;
		break;

	case GAUSSIAN_BALL:
		v = { 0,20,0 };
		a = physx::PxVec3(0, -9.8, 0);
		pose = physx::PxTransform(0, 50, 0);
		mass = 10.0f;
		break;
	}
	d = 0.99;
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1)), &pose, { 0,0.5,0.5,0.3 });
	RegisterRenderItem(renderItem);
}

void Particle::update(double t)
{
	
	// Trivial  case, infinite mass --> do nothing
	//if (inverse_mass <=  0.0f) return;
	// Update position
	pose.p += v * t;
	// Update linear  velocity
	v += (a * mass) * t;
	// Impose drag (damping)
	v *= powf(d, t);

	if (_remaining_time <= 0) exists = false;
	else _remaining_time -= t;
}