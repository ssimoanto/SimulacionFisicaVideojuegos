#pragma once

#include "core.hpp"
#include "RenderUtils.hpp"

enum ParticleType { /*CANYON_BALL,*/ PISTOL_BULLET, FIREWORKS, GAUSSIAN_BALL, BOX_PART, MUELLE, PLANO, LIFE };
class Particle {
protected:

	ParticleType _type;

	double _remaining_time;

	bool exists = true;

	physx::PxTransform pose;

	RenderItem* renderItem;

	Vector3 v;


	double d;
	float mass;
	float inv_mass;
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
	Vector3 a;
	inline Vector3 getPos() { return pose.p; }
	inline Vector3 getVel() { return v; }
	inline void setVelocity(Vector3 vel) { v = vel; }
	inline void setPosition(Vector3 pos) { pose = physx::PxTransform(pos.x, pos.y, pos.z); }
	inline void setTime(double time) { _remaining_time = time; }
	inline bool particleExists() { return exists; }
	inline void setMass(float m) { mass = m; inv_mass = 1 / m; }
	inline float getInv() { return inv_mass; }
	inline float getMass() { return mass; }
	// Accumulated force
	Vector3 force;// Clears accumulated force
	void clearForce() { force *= 0; }// Add force to apply in next integration only
	void addForce(const Vector3& f) { force += f; }
	void kill() { exists = false; }
	void colour(Vector4 col) { renderItem->color = col; }

};