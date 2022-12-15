#pragma once



#include <map>
#include "forceGenerator.h"

typedef std::pair<ForceGenerator*, Particle*> FRPair;

class ParticleForceRegistry : public std::multimap<ForceGenerator*, Particle*> {

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

typedef std::pair<ForceGenerator*, physx::PxRigidDynamic*> FRRPair;


class RigidForceRegistry : public std::multimap<ForceGenerator*, physx::PxRigidDynamic*> {
public:
	void updateForce(double duration) {
		for (auto it = begin(); it != end(); it++)
			it->first->updateForce(it->second, duration);
	}
	void addRegistry(ForceGenerator* fg, physx::PxRigidDynamic* r) {
		this->insert(FRRPair(fg, r));
	}
	void deleteParticleRegistry(physx::PxRigidDynamic* r) {
		auto it = begin();
		while (it != end()) {
			if (it->second != r) {
				it++;
			}
			else {
				it = this->erase(it);
			}
		}
	}
};