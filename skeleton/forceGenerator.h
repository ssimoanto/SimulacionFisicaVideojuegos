#pragma once


#include "particle.h"
#include <list>
#include <random>
#include <iostream>

class ForceGenerator {

public:
	virtual void updateForce(Particle* particle, double duration) = 0;
	virtual void updateForce(physx::PxRigidBody* solid, double duration) {}
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
		Vector3 v = particle->getVel() + f;
		float drag_coef = v.normalize();
		float absdrag = abs(drag_coef);
		Vector3 dragF;
		drag_coef = _k1 * drag_coef + _k2 * absdrag * drag_coef;
		dragF = -v * drag_coef;
		//Apply drag force
		//std::cout << dragF.x << "/t" << dragF.y << "/t" << dragF.z << "/t" << std::endl;
		particle->addForce(dragF);
	}
	 void updateForce(physx::PxRigidBody* solid, double duration) override {
		 if (fabs(solid->getInvMass()) < 1e-10) return;
		 //compute drag force
		 Vector3 v = solid->getLinearVelocity() + f;
		 float drag_coef = v.normalize();
		 float absdrag = abs(drag_coef);
		 Vector3 dragF;
		 drag_coef = _k1 * drag_coef + _k2 * absdrag * drag_coef;
		 dragF = -v * drag_coef;
		 //Apply drag force
		 //std::cout << dragF.x << "/t" << dragF.y << "/t" << dragF.z << "/t" << std::endl;
		 solid->addForce(dragF);
	 }

	void changeWind() { isOn = !isOn; }

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

		Vector3 v = -f + particle->getVel();
		float drag_coef = v.normalize();
		Vector3 dragF;
		drag_coef = _k1 * drag_coef + _k2 * drag_coef * drag_coef;
		dragF = -v * drag_coef;

		particle->addForce(dragF);

	}
	void changeWhirl() { isOn = !isOn; }
};
class ExplosionBoomForceGenerator : public ForceGenerator {
protected:
	float k;
	Vector3 pos;
	double R;


	Vector3 fuerzi;
	float mu = 30.0f;
	double r;


public:

	ExplosionBoomForceGenerator();
	ExplosionBoomForceGenerator(float kConst, double radius, Vector3 posi, float _dur) {
		k = kConst * 1000;
		R = radius * 1000;
		pos = posi;
		tiempo = _dur;
		fuerzi = { 0,0,0 };
	}
	void updateForce(Particle* particle, double t) override {
		if (fabs(particle->getInv()) < 1e-10) return;
		auto newT = GetLastTime() - tiempo;
		if (tiempo <= mu)R += 1000 * tiempo;
		auto particlePos = particle->getPos();

		r = sqrt(pow((particlePos.x - pos.x), 2) + pow((particlePos.y - pos.y), 2) + pow((particlePos.z - pos.z), 2));

		if (r < R)
		{
			auto a = k / pow(r, 2);
			auto b = pow(2.71828182846, -(t / mu));

			fuerzi = Vector3(particlePos.x - pos.x, particlePos.y - pos.y, particlePos.z - pos.z);
			fuerzi *= a * b;
		}

		particle->addForce(fuerzi);
		R = R + 343 * newT;

	}
	 void updateForce(physx::PxRigidBody* solid, double duration) override {
		 if (fabs(solid->getInvMass()) < 1e-10) return;
		 auto newT = GetLastTime() - tiempo;
		 if (tiempo <= mu)R += 1000 * tiempo;
		 auto particlePos = solid->getGlobalPose().p;

		 r = sqrt(pow((particlePos.x - pos.x), 2) + pow((particlePos.y - pos.y), 2) + pow((particlePos.z - pos.z), 2));

		 if (r < R)
		 {
			 auto a = k / pow(r, 2);
			 auto b = pow(2.71828182846, -(t / mu));

			 fuerzi = Vector3(particlePos.x - pos.x, particlePos.y - pos.y, particlePos.z - pos.z);
			 fuerzi *= a * b;
		 }

		 solid->addForce(fuerzi);
		 R = R + 343 * newT;
	}

	float tiempo;
};
class SpringForceGenerator : public ForceGenerator {
public:
	SpringForceGenerator(double k, double resting_length, Particle* other) {
		_k = k;
		_resting_length = resting_length;
		_other = other;
	}
	virtual void updateForce(Particle* particle, double t) {
		Vector3 force = _other->getPos() - particle->getPos();

		const float length = force.normalize();
		const float delta_x = length - _resting_length;

		force *= delta_x * _k;

		particle->addForce(force);
	}

	inline void setk(double k) { _k = k; };

	inline double getK() { return _k; }

	~SpringForceGenerator() = default;

protected:
	double _k;
	double _resting_length;
	Particle* _other;

};
class AnchoredSpringFG : public SpringForceGenerator {
public:
	AnchoredSpringFG(double k, double resting, Vector3 anchor_pos) : SpringForceGenerator(k, resting, nullptr)
	{
		_other = new Particle(BOX_PART, anchor_pos, { 0,0,0 });
	}

	~AnchoredSpringFG() {
		delete _other;
	}
};

class BuoyancyForceGenerator : public ForceGenerator
{
public:
	BuoyancyForceGenerator(float h, float V, float d) {
		_height = h;
		_volume = V;
		_liquid_density = d;

		_liquid_particle = new Particle(PLANO, { 0,15,0 }, { 0,0,0 });
	}

	virtual void updateForce(Particle* particle, double t) {
		float h = particle->getPos().y;
		float h0 = _liquid_particle->getPos().y;

		Vector3 f(0, 0, 0);
		float immersed = 0.0;

		if (h - h0 > _height * 0.5)
		{
			immersed = 0.0;
		}
		else if (h0 - h > _height * 0.5) {
			immersed = 1.0;
		}
		else
		{
			immersed = (h0 - h) / _height + 0.5;
		}

		f.y = _liquid_density * _volume * immersed * 9.8;

		particle->addForce(f);
	}

	virtual ~BuoyancyForceGenerator() = default;

protected:

	float _height;
	float _volume;
	float _liquid_density;
	float _gravity = 9.8;

	Particle* _liquid_particle;
};