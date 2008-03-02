#include "../Vector3.h"
#include "BVHTriangle.h"
#ifndef _BVHBoundingSphere
#define _BVHBoundingSphere

class BVHBoundingSphere
{
private:
	Vector3 ori;
	float radius;
	
	Vector3 getOrigin();
	float getRadius();
public:

	BVHBoundingSphere() {}

	void init(BVHTriangle triangle);
	void init(BVHBoundingSphere g1, BVHBoundingSphere g2);
	float intersect(BVHRay r);
	void glDraw(float c);
};
#endif
