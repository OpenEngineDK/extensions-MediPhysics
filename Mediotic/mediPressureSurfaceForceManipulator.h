#pragma once
#include "mediManipulator.h"
#include "mediBody.h"

class mediPressureSurfaceForceManipulator :	public mediManipulator
{
private:
	float desired_volume;
	float reaction_factor;
	mediBody *body;
public:
	mediPressureSurfaceForceManipulator(mediBody *body, float reaction_factor);
	virtual ~mediPressureSurfaceForceManipulator(void);

	void setDesiredVolume(float desired_volume) {this->desired_volume = desired_volume;}

	void apply();
};
