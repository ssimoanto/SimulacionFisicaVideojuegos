#pragma once

#include "core.hpp"
#include "RenderUtils.hpp"

enum ParticleType { CANYON_BALL, PISTOL_BULLET, FIREWORKS,GAUSSIAN_BALL };
class Particle {
protected:

	ParticleType _type;
	
	double _remaining_time;

	bool exists = true;

	physx::PxTransform pose;
	
	RenderItem* renderItem;

	Vector3 v;
	Vector3 a;

	double d;
	float mass;
public:
	Particle() {};
	Particle(ParticleType type, Vector3 _posi, Vector3 _vel);
	~Particle() {
		DeregisterRenderItem(renderItem);
	}
	void update(double t);
	virtual Particle* clone() const {
		Particle* p = new Particle(_type, pose.p, v);
		return p;
	}
	inline Vector3 getPos() { return pose.p; }
	inline Vector3 getVel() { return v; }
	inline void setVelocity(Vector3 vel) { v = vel; }
	inline void setPosition(Vector3 pos){ pose = physx::PxTransform(pos.x, pos.y,pos.z); }
	inline void setTime(double time) { _remaining_time = time; }
	inline bool particleExists() { return exists; }
	inline void setMass(float m) { mass = m; }
};