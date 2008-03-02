#pragma once
#include "mediManipulator.h"
#include "mediBody.h"

class mediGravityForceManipulator : public mediManipulator
{
private:
	mediBody *body;
	Vector3 gravity_vector;

public:
	mediGravityForceManipulator(mediBody *body, Vector3 gravity_vector);
	virtual ~mediGravityForceManipulator();

	void apply();
	void setGravityVector(Vector3 gravity_vector) { this->gravity_vector=gravity_vector;};
};
