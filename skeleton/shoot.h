#pragma once

#include "core.hpp"
#include "RenderUtils.hpp"
#include "particle.h"
#include <vector>

class Shoot {
	
	// parametros
	Vector3 v;
	Vector3 a;
	Vector3 p;
	double d;
	//
	Vector3 dir;
	Particle* particle;
	
	public:
		std::vector<Particle*> vec;
		Shoot(Vector3 pos, double vel, Vector3 ac, double damping);
		~Shoot() {
			
			for ( auto e: vec)
			{
				delete e;
			}
			vec.clear();
		};
		void create();
		//void integrate(double t);
		//void update(double t);ç
};