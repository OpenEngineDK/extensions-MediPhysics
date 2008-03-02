#include "mediBody.h"
#include "mediManipulator.h"
#include "mediVerletODESolver.h"

mediBody::mediBody(mediVolumeCreationStrategy *v, mediSurfaceCreationStrategy *s)
{
	if (v) v->process(particles);
	if (s) s->process(particles,surface);

	for( unsigned int i=0; i<this->getParticles().size(); i++)
	{
        // the constant defines damping
		this->getParticles()[i].attachODESolver(new mediVerletODESolver(0.05) );
	}

	surface.setBody(this);
	
}

mediBody::~mediBody(void)
{
}

void mediBody::timeStep(float stepsize)
{
	/*for( unsigned int i=0; i<this->getParticles().size(); i++)
	{
		this->getParticles()[i].setForce(Vector3(0,0,0));
	}*/

	for( std::vector<mediManipulator*>::iterator it = forceManipulators.begin(); it!=forceManipulators.end(); it++)
	{
		(*it)->apply();
	}

	for( unsigned int i=0; i<this->getParticles().size(); i++)
	{
		this->getParticles()[i].timeStep(stepsize);
	}

	for( std::vector<mediManipulator*>::iterator it = constraintManipulators.begin(); it!=constraintManipulators.end(); it++)
	{
		(*it)->apply();
	}
}

void mediBody::applyForceManipulator()
{
	for( std::vector<mediManipulator*>::iterator it = forceManipulators.begin(); it!=forceManipulators.end(); it++)
	{
		(*it)->apply();
	}
}


void mediBody::applyTimeStep(float stepsize)
{
	for( unsigned int i=0; i<this->getParticles().size(); i++)
	{
		this->getParticles()[i].timeStep(stepsize);
	}
}

void mediBody::applyContraintManipulator()
{
	for( std::vector<mediManipulator*>::iterator it = constraintManipulators.begin(); it!=constraintManipulators.end(); it++)
	{
		(*it)->apply();
	}

}


mediBoundingSphere mediBody::getBoundingSphere()
{
	Vector3 center; 
	for( unsigned int i=0; i<this->getParticles().size(); i++)
	{
		center += this->getParticles()[i].getPos();;
	}
	center /= this->getParticles().size();

	float radius=0;

	for( unsigned int i=0; i<this->getParticles().size(); i++)
	{
		radius = std::max(radius,(center-this->getParticles()[i].getPos()).length());
	}

	return mediBoundingSphere(radius,center);

}


void mediBody::saveState(){
    initParticles.clear();

	for( unsigned int i=0; i<this->getParticles().size(); i++){
        initParticles.push_back(mediParticle(this->getParticles()[i]));
	}
}

void mediBody::loadState(){
    particles.clear();

	for( unsigned int i=0; i<initParticles.size(); i++){
        particles.push_back(mediParticle(initParticles[i]));
	}
}

