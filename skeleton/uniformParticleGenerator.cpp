#include "uniformParticleGenerator.h"

UniformParticleGenerator::UniformParticleGenerator(std::string _name, Vector3 _posi, Vector3 _vel, Vector3 d_posi, Vector3 d_vel, int _num, ParticleType t) : ParticleGenerator(_name, _posi, _vel)
{
	name = _name;
	_operative = true;
	_pos_width = _posi;
	_vel_width = _vel;
	_num_particles = _num;
	d = std::default_random_engine(randi());

	_model = new Particle(t, _mean_pos, _mean_vel);
}
// uni

std::list<Particle*> UniformParticleGenerator::generateParticles()
{
	std::list<Particle*> lista;

	for (int i = 0; i < _num_particles; i++)
	{
		auto* newPar = _model->clone();
		newPar->setPosition(_model->getPos() + Vector3(uni(d)+ _pos_width.x, uni(d) + _pos_width.y, uni(d) + _pos_width.z));
		newPar->setVelocity(_model->getVel() + Vector3(uni(d) + _vel_width.x, uni(d)+ _vel_width.y, uni(d)+ _vel_width.z));
		/*auto newPar = new Particle(GAUSSIAN_BALL, _model->getPos() + Vector3(normal(d) * std_dev_pos.x, normal(d) * std_dev_pos.y, normal(d) * std_dev_pos.z),
			_model->getVel() + Vector3(normal(d) * std_dev_vel.x, normal(d) * std_dev_vel.y, normal(d) * std_dev_vel.z));*/
		newPar->setTime(1.5);
		lista.push_back(newPar);
	}

	return lista;
}

