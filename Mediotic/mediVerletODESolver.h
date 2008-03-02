#pragma once
#include "mediODESolver.h"

class mediVerletODESolver :
	public mediODESolver
{
private:
	float damping;
	Vector3 oldPos;

public:
	mediVerletODESolver(float damping);
	~mediVerletODESolver(void);

	virtual void setParticle(mediParticle* particle);

	virtual void timeStep(float timestep);

	void setOldPos(Vector3 p) { oldPos = p;} // hack

	Vector3 getOldPos() {return oldPos;}; // hack

/*	void setForce(Vector3 force);
	void addForce(Vector3 force);
	void accelerate(Vector3 a);*/
};
