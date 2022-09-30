#pragma once

#include "core.hpp"
#include "RenderUtils.hpp"


class Particle {
	// Linear velocity in world space
	Vector3 v;
	// Linear acceleration  in world space
	Vector3 a;
	
	double d;
	float mass;
	public:
		Particle(Vector3 Pos, Vector3 Vel, Vector3 Ac, double damping);
		~Particle() {};
		void integrate(double t);
		//void update(double t);
	private:
		physx::PxTransform pose;
		RenderItem* renderItem;
};