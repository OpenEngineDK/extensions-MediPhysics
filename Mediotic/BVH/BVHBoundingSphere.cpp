#include "../Vector3.h"
#include <algorithm>
#include "BVHBoundingSphere.h"

//BoundingSphere

using namespace std;

void BVHBoundingSphere::init(BVHTriangle triangle)
{
	ori = triangle.getCenter();

	float l1,l2,l3;
	l1 = (ori-triangle.nodes[0]).length();
	l2 = (ori-triangle.nodes[1]).length();
	l3 = (ori-triangle.nodes[2]).length();

	radius = max(l1,max(l2,l3));

}
void BVHBoundingSphere::init(BVHBoundingSphere s1, BVHBoundingSphere s2)
{
	ori = (s1.getOrigin()+s2.getOrigin())*0.5;

	Vector3 toS1 = ( s1.getOrigin()-ori );
	Vector3 toS2 = ( s2.getOrigin()-ori );

	Vector3 toS1u = toS1;
	Vector3 toS2u = toS2;
	toS1u.makeUnitVector();
	toS2u.makeUnitVector();

	float lS1 = (toS1 + toS1u*s1.getRadius()).length();
	float lS2 = (toS2 + toS2u*s2.getRadius()).length();

	radius = max(lS1,lS2);

}

Vector3 BVHBoundingSphere::getOrigin()
{
	return ori;
}

float BVHBoundingSphere::getRadius()
{
	return radius;
}

float BVHBoundingSphere::intersect(BVHRay r) {
	Vector3 dst = r.getOrigin() - getOrigin();
	float B = dot(dst,r.getDirection());
	float C = dot(dst,dst) - getRadius()*getRadius();
	float D = B*B - C;
	return D > 0 ? -B - sqrt(D) : numeric_limits<float>::infinity();
//	return D>0 ? true : false;
}

void BVHBoundingSphere::glDraw(float c) {
    /*
      glPushMatrix();
      glTranslatef(getOrigin()[0],getOrigin()[1],getOrigin()[2]);
      //GLUT glutWireSphere(getRadius(),4,4);
      glPopMatrix();
    */
}
