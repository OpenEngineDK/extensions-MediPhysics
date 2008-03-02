#pragma once
#include "mediManipulator.h"
#include "Vector3.h"
#include "mediBody.h"

class mediAxixAlignedBoxConstraintManipulator :
	public mediManipulator
{
private:
	Vector3 low_corner;
	Vector3 high_corner;
	mediBody* body;

	float clamp(float val, float min, float max)
	{
		if (val < min) return min;
		if (val > max) return max;
		return val;
	}

public:
	mediAxixAlignedBoxConstraintManipulator(mediBody* body, Vector3 low_corner, Vector3 high_corner);
	~mediAxixAlignedBoxConstraintManipulator(void);

	void apply();

};
