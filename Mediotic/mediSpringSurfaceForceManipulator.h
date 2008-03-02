#pragma once
#include "mediManipulator.h"
#include "mediBody.h"

class mediSpringSurfaceForceManipulator : public mediManipulator
{
private:
	mediBody *body;
	float spring_constant;

public:
	mediSpringSurfaceForceManipulator(mediBody *body, float spring_constant);
	virtual ~mediSpringSurfaceForceManipulator(void);

	void apply();
};
