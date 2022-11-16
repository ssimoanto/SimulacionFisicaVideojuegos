#pragma once


#include "particle.h"
#include <list>
#include <random>
#include <iostream>

class ForceGenerator {

public:
	virtual void updateForce(Particle* particle, double duration) = 0;
	std::string _name;
	double t = -1e10;
	bool isOn = false;

};
class GravityForceGenerator : public ForceGenerator {
public:
	GravityForceGenerator(const Vector3& g) { _gravity = g; }
	virtual void updateForce(Particle* p, double t) {
		if (fabs(p->getInv()) < 1e-10) return;
		p->addForce(_gravity * p->getMass());
	}
	inline void setGravity(Vector3 g) {
		_gravity = g;
	}
protected:
	Vector3 _gravity;

};

class ParticleDragGenerator : public ForceGenerator {
public:
	ParticleDragGenerator();
	ParticleDragGenerator(const float k1, const float k2) {
		_k1 = k1;
		_k2 = k2;
	}

	virtual void updateForce(Particle* particle, double t) {
		if (fabs(particle->getInv()) < 1e-10) return;
		//compute drag force
		Vector3 v = particle->getVel();
		float drag_coef = v.normalize();
		Vector3 dragF;
		drag_coef = _k1 * drag_coef + _k2 * drag_coef * drag_coef;
		dragF = -v * drag_coef;
		//Apply drag force
		std::cout << dragF.x << "/t" << dragF.y << "/t" << dragF.z << "/t" << std::endl;
		particle->addForce(dragF);
	}
	inline void setDrag(float k1, float k2) {
		_k1 = k1;
		_k2 = k2;
	}
	inline float getK1() { return _k1; }
	inline float getK2() { return _k2; }
protected:
	float _k1;
	float _k2;
};
class WindOfChangeForceGenerator : public ParticleDragGenerator {
public:
	WindOfChangeForceGenerator(Vector3 fuerzaViento, const float k1, const float k2) : ParticleDragGenerator(k1, k2) {
		f = fuerzaViento;
	}
	Vector3 f;
	void updateForce(Particle* particle, double t) override {
		if (fabs(particle->getInv()) < 1e-10) return;
		//compute drag force
		Vector3 v = particle->getVel()+f;
		float drag_coef = v.normalize();
		Vector3 dragF;
		drag_coef = _k1 * drag_coef + _k2 * drag_coef * drag_coef;
		dragF = -v * drag_coef;
		//Apply drag force
		//std::cout << dragF.x << "/t" << dragF.y << "/t" << dragF.z << "/t" << std::endl;
		particle->addForce(dragF);
	}

};
class WhirlwindOfChangeForceGenerator : public WindOfChangeForceGenerator {
protected:
	float k;
	Vector3 whirlPosi;
public:
	WhirlwindOfChangeForceGenerator();
	WhirlwindOfChangeForceGenerator(const float _k, Vector3 p, Vector3 v) : WindOfChangeForceGenerator(v, _k, 0) {
		k = _k;
		whirlPosi = p;
	}
	void updateForce(Particle* particle, double t) override {
		if (fabs(particle->getInv()) < 1e-10) return;

		auto posi = particle->getPos();
		f = { k * (-(posi.z - whirlPosi.z)),k * (75 - (posi.y - whirlPosi.y)),k * (posi.x - whirlPosi.x) };

		Vector3 v = -f+particle->getVel();
		float drag_coef = v.normalize();
		Vector3 dragF;
		drag_coef = _k1 * drag_coef + _k2 * drag_coef * drag_coef;
		dragF = -v * drag_coef;
		
		particle->addForce(dragF);

	}
};