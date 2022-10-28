#include "uniformParticleGenerator.h"

UniformParticleGenerator::UniformParticleGenerator(Vector3 _posi, Vector3 _vel) : ParticleGenerator("Uniform", _posi, _vel)
{

	_operative = true;
	_pos_width = _posi;
	_vel_width = _vel;
	_num_particles = 7;
	d = std::default_random_engine(randi());
	_model = new Particle(GAUSSIAN_BALL, _pos_width, _vel_width);
	_model->setVelocity({ 0,0,27 });
}
// uni

std::list<Particle*> UniformParticleGenerator::generateParticles()
{
	std::list<Particle*> lista;

	for (int i = 0; i < _num_particles; i++)
	{
		auto* newPar = _model->clone();
		newPar->setPosition(_model->getPos() + Vector3(uni(d) * _pos_width.x, uni(d) * _pos_width.y, uni(d) * _pos_width.z));
		newPar->setVelocity(_model->getVel() + Vector3(uni(d) * _vel_width.x, uni(d) * _vel_width.y, uni(d) * _vel_width.z));
		/*auto newPar = new Particle(GAUSSIAN_BALL, _model->getPos() + Vector3(normal(d) * std_dev_pos.x, normal(d) * std_dev_pos.y, normal(d) * std_dev_pos.z),
			_model->getVel() + Vector3(normal(d) * std_dev_vel.x, normal(d) * std_dev_vel.y, normal(d) * std_dev_vel.z));*/
		newPar->setTime(1.5);
		lista.push_back(newPar);
	}
	return lista;
}

