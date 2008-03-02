#include "mediAxixAlignedBoxConstraintManipulator.h"

mediAxixAlignedBoxConstraintManipulator::mediAxixAlignedBoxConstraintManipulator(mediBody* body, Vector3 low_corner, Vector3 high_corner)
: low_corner(low_corner), high_corner(high_corner), body(body)
{
}

mediAxixAlignedBoxConstraintManipulator::~mediAxixAlignedBoxConstraintManipulator(void)
{
}

void mediAxixAlignedBoxConstraintManipulator::apply(void)
{
	std::vector<mediParticle>& particles = body->getParticles();

	std::vector<mediParticle>::iterator it;
	for(it = particles.begin(); it!=particles.end(); it++)
	{
		Vector3 pos = (*it).getPos();

		pos[0] = clamp(pos[0], low_corner[0], high_corner[0]);
		pos[1] = clamp(pos[1], low_corner[1], high_corner[1]);
		pos[2] = clamp(pos[2], low_corner[2], high_corner[2]);

		(*it).setPos(pos);
	}
}
