#include "../Vector3.h"
#include <algorithm>
#include "BVHBoundingBox.h"
//GLUT #include <Meta/GLUT.h>

//BoundingSphere
void BVHBoundingBox::init(BVHTriangle triangle)
{
	for(int i=0;i<3;i++) 
		maxB.e[i] = std::max(triangle.nodes[0][i],std::max(triangle.nodes[1][i],triangle.nodes[2][i]));
	for(int i=0;i<3;i++) 
		minB.e[i] = std::min(triangle.nodes[0][i],std::min(triangle.nodes[1][i],triangle.nodes[2][i]));
}
void BVHBoundingBox::init(BVHBoundingBox s1, BVHBoundingBox s2)
{
	for(int i=0;i<3;i++) 
		maxB.e[i] = std::max(s1.getMaxB()[i],s2.getMaxB()[i]);
	for(int i=0;i<3;i++) 
		minB.e[i] = std::min(s1.getMinB()[i],s2.getMinB()[i]);
}


bool BVHBoundingBox::intersect(BVHRay r) 
{
	return intersectInDirection(r) || intersectInDirection(BVHRay(r.getOrigin(),r.getDirection()*-1));	
}

#define RIGHT	0
#define LEFT	1
#define MIDDLE	2
bool BVHBoundingBox::intersectInDirection(BVHRay r)
{
//Adapted from 
//Fast Ray-Box Intersection
//by Andrew Woo
//from "Graphics Gems", Academic Press, 1990



	bool inside = true;
	char quadrant[3];
	register int i;
	int whichPlane;
	double maxT[3];
	double candidatePlane[3];
	double coord[3];

	/* Find candidate planes; this loop can be avoided if
   	rays cast all from the eye(assume perpsective view) */
	for (i=0; i<3; i++)
		if(r.getOrigin()[i] < minB[i]) {
			quadrant[i] = LEFT;
			candidatePlane[i] = minB[i];
			inside = false;
		}else if (r.getOrigin()[i] > maxB[i]) {
			quadrant[i] = RIGHT;
			candidatePlane[i] = maxB[i];
			inside = false;
		}else	{
			quadrant[i] = MIDDLE;
		}

	/* Ray origin inside bounding box */
	if(inside)	{
		//coord = origin;
		return (true);
	}

	// stupid addition to check for rays in backwards also

	/* Calculate T distances to candidate planes */
	for (i = 0; i < 3; i++)
		if (quadrant[i] != MIDDLE && r.getDirection()[i] !=0.)
			maxT[i] = (candidatePlane[i]-r.getOrigin()[i]) / r.getDirection()[i];
		else
			maxT[i] = -1.;

	/* Get largest of the maxT's for final choice of intersection */
	whichPlane = 0;
	for (i = 1; i < 3; i++)
		if (maxT[whichPlane] < maxT[i])
			whichPlane = i;

	/* Check final candidate actually inside box */
	if (maxT[whichPlane] < 0.) return (false);
	for (i = 0; i < 3; i++)
		if (whichPlane != i) 
		{
			coord[i] = r.getOrigin()[i] + maxT[whichPlane] * r.getDirection()[i];
			if (coord[i] < minB[i] || coord[i] > maxB[i])
				return (false);
		}
		else
		{
			coord[i] = candidatePlane[i];
		}
	return (true);				/* ray hits box */

}

void BVHBoundingBox::glDraw(float c) {

}




