#include "../Vector3.h"
#include "BVHTriangle.h"

#ifndef _BVHBoundingBox
#define _BVHBoundingBox

class BVHBoundingBox
{
private:
	Vector3 minB;
	Vector3 maxB;
	
	Vector3 getMinB() {return minB;}
	Vector3 getMaxB() {return maxB;}

	bool intersectInDirection(BVHRay r);
public:

	BVHBoundingBox() {}
	void init(BVHTriangle triangle);
	void init(BVHBoundingBox g1, BVHBoundingBox g2);
	bool intersect(BVHRay r);
	void glDraw(float c);
};
#endif
