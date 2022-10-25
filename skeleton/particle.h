#pragma once

#include "core.hpp"
#include "RenderUtils.hpp"

enum ParticleType { CANYON_BALL, PISTOL_BULLET, FIREWORKS,GAUSSIAN_BALL };
class Particle {
private:

	ParticleType _type;
	
	double _remaining_time;

	bool exists = true;

	physx::PxTransform pose;
	
	RenderItem* renderItem;

	// Linear velocity in world space
	Vector3 v;
	// Linear acceleration  in world space
	Vector3 a;

	double d;
	float mass;
public:
	Particle(ParticleType type);
	Particle(Vector3 _posi, Vector3 _vel) {
		pose = physx::PxTransform(_posi);
		v = _vel;
		a = physx::PxVec3(0, -9.8, 0);
		mass = 10.0f;
		d = 0.99;
		renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1.0)), &pose, { 0,0.5,0.5,0.3 });
		RegisterRenderItem(renderItem);

	}
	~Particle() { DeregisterRenderItem(renderItem); }
	void update(double t);
	virtual Particle* clone() const {
		Particle* p = new Particle(pose.p,v);
		return p;
	}
	inline Vector3 getPos() { return pose.p; }
	inline Vector3 getVel() { return v; }
	inline void setTime(double time) { _remaining_time = time; }
	inline bool particleExists() { return exists; }
};