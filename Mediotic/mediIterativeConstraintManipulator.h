#pragma once
#include "mediManipulator.h"
#include "mediBody.h"

class mediIterativeConstraintManipulator :	public mediManipulator
{
public:
	class Constraint
	{
		mediParticle *p1;
		mediParticle *p2;
		float originalLength;
		
	public:
		Constraint(mediParticle *p2,mediParticle *p1);
		float apply();

	};


private:
	mediBody *body;
	vector<Constraint> constraints;
	int no_iterations;

public:
	mediIterativeConstraintManipulator(mediBody *body, int no_iterations,float threshold=0.15);
	mediIterativeConstraintManipulator(mediBody *body, mediBody *connectToBody, int no_iterations,float threshold);
	virtual ~mediIterativeConstraintManipulator(void);
	void apply();
};
