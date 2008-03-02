#include "mediODESolver.h"
#include "mediParticle.h"

mediODESolver::mediODESolver(void)
{
}

mediODESolver::~mediODESolver(void)
{
}

void mediODESolver::setParticle(mediParticle* particle) 
{
	my_particle = particle;
}

void mediODESolver::timeStep(float timestep) {}

void mediODESolver::setForce(Vector3 force) {this->acc = force*my_particle->getInvMass();}
void mediODESolver::addForce(Vector3 force) {this->acc += force*my_particle->getInvMass();}

void mediODESolver::accelerate(Vector3 a) 
{
		acc += a;
}
