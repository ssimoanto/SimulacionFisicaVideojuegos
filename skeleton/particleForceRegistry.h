#pragma once



#include <map>
#include "forceGenerator.h"

typedef std::pair<ForceGenerator*, Particle*> FRPair;

class ParticleForceRegistry: public std::multimap<ForceGenerator*,Particle*> {

public:
	inline void updateForce(double duration) {
		for (auto it = begin(); it != end(); it++)
			it->first->updateForce(it->second, duration);
	}
	inline void addRegistry(ForceGenerator* fg, Particle* p) {
		this->insert(FRPair(fg, p));
	}
	inline void deleteParticleRegistry(Particle* p) {
		auto it = begin();
		while (it != end()) {
			if (it->second != p) it++;
			else it = this->erase(it);
		}
	}

};