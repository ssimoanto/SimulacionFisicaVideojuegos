#include "particle.h"

Particle::Particle(ParticleType type, Vector3 _posi, Vector3 _vel)
{
	// Vector3 Pos, Vector3 Vel, Vector3 Ac, double damping
	_type = type;
	switch (_type) {
	case FIREWORKS:
		a = physx::PxVec3(0, -10.0, 0);
		mass = 2.5f;
		inv_mass = 1 / mass;
		pose = physx::PxTransform(_posi);
		v = _vel;
		break;
	case CANYON_BALL:
		v = 200 * GetCamera()->getDir();
		a = physx::PxVec3(0, -9.8, 0);
		pose = physx::PxTransform(GetCamera()->getEye().x, GetCamera()->getEye().y, GetCamera()->getEye().z);
		mass = 17.0f;
		inv_mass = 1 / mass;
		break;

		// cañon: 17,48 cm, masa 17,6 kg, distancia 3700 m
		// https://es.wikipedia.org/wiki/Bola_de_ca%C3%B1%C3%B3n
		// artilleria naval
		// hacer un vector de particulas para poder dispararlas
		// luego habra que hacer delete de cada elemento pq son punteros
	case PISTOL_BULLET:
		v = 400 * GetCamera()->getDir();
		a = physx::PxVec3(0, -9.8, 0);
		pose = physx::PxTransform(GetCamera()->getEye().x, GetCamera()->getEye().y, GetCamera()->getEye().z);
		mass = 10.0f;
		inv_mass = 1 / mass;
		break;

	case GAUSSIAN_BALL:
		a = physx::PxVec3(0, -10.0, 0);
		mass = 10.0f;
		inv_mass = 1 / mass;
		pose = physx::PxTransform(_posi);
		v = _vel;
		break;
	}
	d = 0.99;
	exists = true;
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1)), &pose, { 0,0.5,0.5,1 });
}

void Particle::update(double t)
{
	// Trivial case, infinite mass --> do nothing
	if (inv_mass<= 0.0f) return;
	// Update position
	pose.p += v * t;
	// Update linear  velocity
	v += (a * mass) * t;
	// Impose drag (damping)
	//v *= powf(d, t);

	Vector3 totalAcceleration = a; totalAcceleration += force * inv_mass;

	v += totalAcceleration * t;// Impose drag (damping)
	v *= powf(d, t);clearForce();

	if (_remaining_time <= 0) exists = false;
	else _remaining_time -= t;
}
