#include "gaussianParticleGenerator.h"

// posicion, velocidad, desviacion de pos y vel, numero de particulas por frame
GaussianParticleGenerator::GaussianParticleGenerator(std::string _name, Vector3 _posi, Vector3 _vel,
	Vector3 d_posi, Vector3 d_vel, int _num, ParticleType t) : ParticleGenerator(_name, _posi, _vel)
{
	name = _name;
	_operative = true;
	_mean_pos = _posi;
	_mean_vel = _vel;
	std_dev_pos = d_posi;
	std_dev_vel = d_vel;
	_num_particles = _num;
	d = default_random_engine(randi());
	_model = new Particle(t, _mean_pos, _mean_vel);
		
	if (name != "Firework") {
		posAux = _model->getPos();
		_model->setPosition({ 3877483748737483,0,0 });
	}
}
std::list<Particle*> GaussianParticleGenerator::generateParticles()
{
	std::list<Particle*> lista;

	for (int i = 0; i < _num_particles; i++)
	{
		auto* newPar = _model->clone();
		
		if (name != "Firework")newPar->setPosition(posAux + Vector3(normal(d) * std_dev_pos.x, normal(d) * std_dev_pos.y, normal(d) * std_dev_pos.z));
		else newPar->setPosition(_model->getPos() + Vector3(normal(d) * std_dev_pos.x, normal(d) * std_dev_pos.y, normal(d) * std_dev_pos.z));
		newPar->setVelocity(_model->getVel() + Vector3(normal(d) * std_dev_vel.x, normal(d) * std_dev_vel.y, normal(d) * std_dev_vel.z));
		/*auto newPar = new Particle(GAUSSIAN_BALL, _model->getPos() + Vector3(normal(d) * std_dev_pos.x, normal(d) * std_dev_pos.y, normal(d) * std_dev_pos.z),
			_model->getVel() + Vector3(normal(d) * std_dev_vel.x, normal(d) * std_dev_vel.y, normal(d) * std_dev_vel.z));*/
		newPar->setTime(1.5);
		lista.push_back(newPar);
	}

	return lista;
}

