#include "mediGravityForceManipulator.h"

	
mediGravityForceManipulator::mediGravityForceManipulator(mediBody *body, Vector3 gravity_vector) 
: body(body), gravity_vector(gravity_vector)
{
}

mediGravityForceManipulator::~mediGravityForceManipulator(void)
{
}

void mediGravityForceManipulator::apply()
{
	std::vector<mediParticle>& particles = body->getParticles();

	std::vector<mediParticle>::iterator it;
	for(it = particles.begin(); it!=particles.end(); it++)
	{
		(*it).accelerate(gravity_vector);
	}

}
