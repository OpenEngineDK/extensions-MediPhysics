#pragma once
#include "mediManipulator.h"
#include "mediBody.h"

class mediPlasticSpringAllForceManipulator : public mediManipulator
{
	class Spring
	{
		mediParticle *p1;
		mediParticle *p2;
		float restLength;		
	public:
		Spring(mediParticle *p2,mediParticle *p1);
		void applyForce(float spring_constant, float deforming_factor);
		void prolongSpring(float amount);

	};


private:
	float spring_constant;
    float deforming_factor;

	mediBody *body;
	vector<Spring> springs;

public:
	mediPlasticSpringAllForceManipulator(mediBody *body, float spring_constant, float threshold, float deforming_factor);
	virtual ~mediPlasticSpringAllForceManipulator(void);
	void apply();
};
