#pragma once

#include "core.hpp"
#include "particle.h"
#include <list>
#include <string>
#include "RenderUtils.hpp"

class ParticleGenerator {
protected:
	std::string name;

	Vector3 _mean_pos;
	Vector3 _mean_vel;

	double _generation_probability;

	int _num_particles;

	Particle* _model;

	bool _operative;
public:
	ParticleGenerator(){}
	ParticleGenerator(std::string _name, Vector3 _posi, Vector3 _vel);
	~ParticleGenerator() {};
	void setParticle(Particle* model);
	virtual std::string getGeneratorName();
	// = 0 -> virtual puro, hay que sobreescribirlo si o si
	virtual std::list<Particle*> generateParticles() = 0;
	inline void setMeanPos(Vector3 p) { _mean_pos = p; }
	inline bool isOperative() { return _operative; }
	inline void changeOperative() { _operative = !_operative; }
	inline void putNum(int n) { _num_particles = n; }

};

class CircleGenerator : public ParticleGenerator
{
	int max_, min_;
	int fws;
	const float PI = 3.1415;
public:
	CircleGenerator(int max, int min): ParticleGenerator() {
		max_ = max;
		min_ = min;
		fws = 10;
		name = "CIRCLE";
	}
	~CircleGenerator() = default;


	std::list<Particle*>generateParticles() override {

		std::list<Particle*>x;
		auto random = rand() % max_ + min_;
		auto angle = 90;
		for (int i = 0; i < fws; i++)
		{
			auto p = _model->clone();

			p->setPosition(_model->getPos());
			p->setVelocity(Vector3(_model->getVel().x + random * cos(angle * PI / 180.0),
				_model->getVel().y, _model->getVel().z + random * sin(angle * PI / 180.0)));
			angle += 360.0 / fws;

			x.push_back(p);
		}

		return x;
	}

};