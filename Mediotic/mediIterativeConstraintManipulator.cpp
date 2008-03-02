#include "mediIterativeConstraintManipulator.h"
#include "mediParticle.h"

mediIterativeConstraintManipulator::mediIterativeConstraintManipulator(mediBody *body, int no_iterations,float threshold) 
: body(body), no_iterations(no_iterations)
{
	;
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
					constraints.push_back(Constraint(&p1,&p2));
			}
}

mediIterativeConstraintManipulator::mediIterativeConstraintManipulator(mediBody *body, mediBody *connectToBody, int no_iterations,float threshold) 
: body(body), no_iterations(no_iterations)
{
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
					constraints.push_back(Constraint(&p1,&p2));
			}
}

mediIterativeConstraintManipulator::~mediIterativeConstraintManipulator(void)
{
}

void mediIterativeConstraintManipulator::apply()
{
	for (int i = 0; i<no_iterations; i++)
	for(vector<Constraint>::iterator it = constraints.begin(); it != constraints.end(); it++)
	{
		Constraint s = (*it);
		s.apply();
	}
}

mediIterativeConstraintManipulator::Constraint::Constraint(mediParticle *p1,mediParticle *p2) : p1(p1), p2(p2) 
{
originalLength =(p1->getPos()-p2->getPos()).length();

};

float mediIterativeConstraintManipulator::Constraint::apply()
{
	Vector3 vec = (p1->getPos()-p2->getPos());
	float currentLength = vec.length();
	vec.makeUnitVector();
	Vector3 amount = (vec*(currentLength-originalLength)/2.0);
	p1->setPos(p1->getPos()-amount);
	p2->setPos(p2->getPos()+amount);
	return amount.length();
}
