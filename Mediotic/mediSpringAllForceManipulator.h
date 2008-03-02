#pragma once
#include "mediManipulator.h"
#include "mediBody.h"

class mediSpringAllForceManipulator : public mediManipulator
{
public:
	class Spring
	{
		mediParticle *p1;
		mediParticle *p2;
		float restLength;
		
	public:
		Spring(mediParticle *p2,mediParticle *p1);
		void applyForce(float spring_constant);

	};


private:
	float spring_constant;
	mediBody* body;
	vector<Spring> springs;

public:
	mediSpringAllForceManipulator(mediBody *body, float spring_constant, float threshold);
	mediSpringAllForceManipulator(mediBody *body, mediBody *connectToBody, float spring_constant, float threshold); // connect to another body
	virtual ~mediSpringAllForceManipulator(void);
	void apply();
};
