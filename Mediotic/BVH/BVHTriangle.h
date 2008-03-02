#include "../Vector3.h"
#include <limits>
#include "BVHRay.h"

#ifndef _BVHTriangle
#define _BVHTriangle



class BVHTriangle
{
public:
	int id;
	Vector3 nodes[3];

	BVHTriangle() { id=-1;};
	BVHTriangle(int id, Vector3 a, Vector3 b, Vector3 c);
	Vector3 getCenter();
	bool intersect(BVHRay r, float *dist);
};

class BVHTriangleHit
{
public:
	float distance;
	Vector3 coord;
	BVHTriangle triangle;
	BVHTriangleHit()
	{
		distance = std::numeric_limits<float>::infinity();
	}

	BVHTriangleHit(float distance, BVHTriangle triangle, Vector3 coord)
	{
		this->distance = distance;
		this->triangle = triangle;
		this->coord = coord;

		//calculate barycentric coordiantes
	}

	Vector3 getBarycentrics()
	{

		// barycentric coordinates to x,y in triangle a,b,c
		// u*a +v*b + (1-u-v)*c

		float abcArea = (cross(triangle.nodes[0]-triangle.nodes[1],triangle.nodes[2]-triangle.nodes[1])).length();

		float u = (cross(triangle.nodes[1]-coord,triangle.nodes[2]-coord)).length() / abcArea;
		float v = (cross(triangle.nodes[0]-coord,triangle.nodes[2]-coord)).length() / abcArea;

		return Vector3(u,v,1-u-v);

	}

};

bool OrderLessX(BVHTriangle left, BVHTriangle right);

bool OrderLessY(BVHTriangle left, BVHTriangle right);

bool OrderLessZ(BVHTriangle left, BVHTriangle right);

#endif
