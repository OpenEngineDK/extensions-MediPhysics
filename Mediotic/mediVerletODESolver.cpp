#include "mediVerletODESolver.h"
#include "mediParticle.h"

mediVerletODESolver::mediVerletODESolver(float damping) : damping(damping)
{
	mediODESolver();
}

mediVerletODESolver::~mediVerletODESolver(void)
{
}

void mediVerletODESolver::setParticle(mediParticle* particle) 
{
	mediODESolver::setParticle(particle); //call constructor of parent class
	//this->my_particle = particle;
	oldPos = particle->getPos(); //initialize old position
}

void mediVerletODESolver::timeStep(float timestep)
{
	//Optimize this?
	Vector3 res = (2.0f-damping)*my_particle->getPos();
	res -= (1.0f-damping)*oldPos;
	res += acc*timestep*timestep;
	oldPos = my_particle->getPos();
	my_particle->setPos(res);

}

