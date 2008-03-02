#include "../Vector3.h"
#include "BVHRay.h"


#ifndef _BVHray
#define _BVHRay
// Ray
BVHRay::BVHRay(Vector3 origin, Vector3 direction)
{
	this->ori = origin;
	this->dir = direction;
}

Vector3 BVHRay::getDirection()
{
	return dir;
}

Vector3 BVHRay::getOrigin()
{
	return ori;
}
#endif
