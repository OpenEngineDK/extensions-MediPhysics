#include "mediSpringAllForceManipulator.h"
#include "mediParticle.h"

mediSpringAllForceManipulator::mediSpringAllForceManipulator(mediBody *body, float spring_constant, float threshold) 
: spring_constant(spring_constant)
{
    this->body = body;
	vector<mediParticle>& particles =  body->getParticles();

	vector<mediParticle>::iterator it1;
	vector<mediParticle>::iterator it2;

	for(it1=particles.begin();it1!=particles.end();it1++)
		for(it2=particles.begin();it2!=particles.end();it2++)
			if (it1!=it2 && ((*it1).getPos()-(*it2).getPos()).length()<threshold )
			{
				mediParticle &p1 = (*it1);
				mediParticle &p2 = (*it2);
				
				Vector3 vec = (p1.getPos()-p2.getPos());

				if ( vec.e[0] > 0 || (vec.e[0]==0 && vec.e[1] > 0 ) || (vec.e[0]==0 && vec.e[1]==0 &&vec.e[2]>0))
					springs.push_back(Spring(&p1,&p2));
			}
}

mediSpringAllForceManipulator::mediSpringAllForceManipulator(mediBody *body, mediBody *connectToBody, float spring_constant, float threshold)
: spring_constant(spring_constant)
{
	this->body = NULL;
	vector<mediParticle>& particles =  body->getParticles();
	vector<mediParticle>& connectToBodyParticles =  connectToBody->getParticles();

	vector<mediParticle>::iterator it1;
	vector<mediParticle>::iterator it2;

	for(it1=particles.begin();it1!=particles.end();it1++)
		for(it2=connectToBodyParticles.begin();it2!=connectToBodyParticles.end();it2++)
			if (((*it1).getPos()-(*it2).getPos()).length()<threshold )
			{
				mediParticle &p1 = (*it1);
				mediParticle &p2 = (*it2);

				Vector3 vec = (p1.getPos()-p2.getPos());

				if ( vec.e[0] > 0 || (vec.e[0]==0 && vec.e[1] > 0 ) || (vec.e[0]==0 && vec.e[1]==0 &&vec.e[2]>0))
					springs.push_back(Spring(&p1,&p2));
			}
}

mediSpringAllForceManipulator::~mediSpringAllForceManipulator(void)
{
}

void mediSpringAllForceManipulator::apply()
{
	for(vector<Spring>::iterator it = springs.begin(); it != springs.end(); it++)
	{
		Spring s = (*it);
		s.applyForce(spring_constant);
	}
}

mediSpringAllForceManipulator::Spring::Spring(mediParticle *p1,mediParticle *p2) : p1(p1), p2(p2) 
{
	restLength =(p1->getPos()-p2->getPos()).length();
};

void mediSpringAllForceManipulator::Spring::applyForce(float spring_constant)
{
	Vector3 vec = (p1->getPos()-p2->getPos());
	float currentLength = vec.length();
	vec.makeUnitVector();
	Vector3 amount = (vec*(currentLength-restLength)/2.0)*spring_constant;
	p1->addForce(-amount);
	p2->addForce(amount);
}
