#include "mediFace.h"
#include "mediParticle.h"

mediFace::mediFace(void)
{
}

mediFace::~mediFace(void)
{
}

mediFace::mediFace(mediParticle *p1, mediParticle *p2, mediParticle *p3)
{
	particles[0] = p1;
	particles[1] = p2;
	particles[2] = p3;

	original_side_lengths[0] = (p2->getPos() - p1->getPos()).length();
	original_side_lengths[1] = (p3->getPos() - p2->getPos()).length();
	original_side_lengths[2] = (p1->getPos() - p3->getPos()).length();
}

Vector3 mediFace::getNormal()
{
	Vector3 &v1 = particles[0]->getPos();
	Vector3 &v2 = particles[1]->getPos();
	Vector3 &v3 = particles[2]->getPos();

	Vector3 tmp = (cross(v2-v1, v3-v1));
	tmp.makeUnitVector();
/*
	if (tmp.squaredLength()>1.001 ||  tmp.squaredLength()<0.999)
	{
		tmp = Vector3(0,0,0);
	}
*/
	return tmp;
}
