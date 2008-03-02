#include "mediPlasticSpringAllForceManipulator.h"
#include "mediParticle.h"

mediPlasticSpringAllForceManipulator::mediPlasticSpringAllForceManipulator(mediBody *body, float spring_constant, float threshold, float deforming_factor) {
  this->body = body;
  this->spring_constant = spring_constant;
  this->deforming_factor = deforming_factor;
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

mediPlasticSpringAllForceManipulator::~mediPlasticSpringAllForceManipulator(void)
{
}

void mediPlasticSpringAllForceManipulator::apply()
{
	for(vector<Spring>::iterator it = springs.begin(); it != springs.end(); it++)
	{
		Spring s = (*it);
		s.applyForce(spring_constant, deforming_factor);
	}
}

mediPlasticSpringAllForceManipulator::Spring::Spring(mediParticle *p1,mediParticle *p2) : p1(p1), p2(p2) 
{
restLength =(p1->getPos()-p2->getPos()).length();

};

void mediPlasticSpringAllForceManipulator::Spring::applyForce(float spring_constant, float deforming_factor)
{
	Vector3 vec = (p1->getPos()-p2->getPos());
	float currentLength = vec.length();
	vec.makeUnitVector();
	float delta_length = currentLength-restLength;
	if (delta_length!=0)
	{
		prolongSpring(deforming_factor*delta_length);
		Vector3 amount = (vec*(deforming_factor*delta_length)/2.0)*spring_constant;
		p1->addForce(-amount);
		p2->addForce(amount);
	}
}

void mediPlasticSpringAllForceManipulator::Spring::prolongSpring(float amount)
{
	restLength += amount;
//	assert(amount == 0);
}
