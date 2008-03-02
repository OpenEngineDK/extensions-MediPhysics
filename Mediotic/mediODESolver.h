#pragma once

#include "Vector3.h"


#define mediODESolver_epsilon 0.00000001f

class mediParticle;

class mediODESolver
{
protected:
	Vector3 acc;					/**< acceleration of associated particle */

public:
	mediParticle* my_particle;		/**< associated particle */
	mediODESolver();
	virtual ~mediODESolver(void);

	virtual void setParticle(mediParticle* particle);

	virtual void timeStep(float timestep);
	
	void setForce(Vector3 force);
	void addForce(Vector3 force);

	void accelerate(Vector3 a);
};
