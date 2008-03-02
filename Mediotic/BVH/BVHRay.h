#ifndef _BVHRay
#define _BVHRay

class BVHRay
{
private:
	Vector3 ori;
	Vector3 dir;
	
public:
	BVHRay(Vector3 origin, Vector3 direction);
	Vector3 getDirection();
	Vector3 getOrigin();

};

#endif
