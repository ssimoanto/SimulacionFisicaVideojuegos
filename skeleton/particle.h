#pragma once

#include "core.hpp"
#include "RenderUtils.hpp"


class Particle {
	// Linear velocity in world space
	Vector3 v;
	// Linear acceleration  in world space
	//Vector3 a;
	public:
		Particle(Vector3 Pos, Vector3 Vel);
		~Particle();
		void integrate(double t);
		//void update(double t);
	private:
		physx::PxTransform pose;
		RenderItem* renderItem;
};