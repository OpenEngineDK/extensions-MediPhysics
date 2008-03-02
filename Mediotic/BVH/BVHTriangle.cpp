#include "../Vector3.h"
#include "BVHTriangle.h"
//Triangle

BVHTriangle::BVHTriangle(int id, Vector3 a, Vector3 b, Vector3 c)
{
	nodes[0] = a;
	nodes[1] = b;
	nodes[2] = c;
	this->id = id;
}


bool BVHTriangle::intersect(BVHRay r, float *dist)
{
	// FROM Moller: "Realtime Rendering"

	float EPS = 0.0000001;

	Vector3 e1 = nodes[1]-nodes[0];
	Vector3 e2 = nodes[2]-nodes[0];
	Vector3 d = r.getDirection();
	Vector3 o = r.getOrigin();

	Vector3 p;
	p=cross(d,e2);
	float a = dot(e1,p);
	if (a>-EPS && a<EPS) return false;
	float f = 1/a;
	Vector3 s = o-nodes[0];
	float u = f*(dot(s,p));
	if (u<0.0 || u >1.0) return false;
	Vector3 q;
	q=cross(s,e1);
	float v = f*(dot(d,q));
	if (v<0.0 || u+v>1.0) return false;
	float t = f*(dot(e2,q));
	if (t<0) return false;
	*dist = t;
	return true;


}

Vector3 BVHTriangle::getCenter()
{
	return (nodes[0] + nodes[1] + nodes[2])/3.0;
}

bool OrderLessX(BVHTriangle left, BVHTriangle right)
{
	return left.getCenter()[0]<right.getCenter()[0];
};

bool OrderLessY(BVHTriangle left, BVHTriangle right)
{
	return left.getCenter()[1]<right.getCenter()[1];
};

bool OrderLessZ(BVHTriangle left, BVHTriangle right)
{
	return left.getCenter()[2]<right.getCenter()[2];
};





