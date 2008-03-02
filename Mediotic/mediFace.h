#pragma once
//#include "mediParticle.h"
#include "Vector3.h"

class mediParticle;

class mediFace
{
private:
	mediParticle* particles[3];
	Vector3 original_side_lengths; //!< The original lengths of the 3 sides in the triangle

public:
	mediFace(void);
	mediFace(mediParticle *p1, mediParticle *p2, mediParticle *p3);
    mediParticle& operator[](int i) {  return *(particles[i]); }
	Vector3 getNormal();
	~mediFace(void);
	float getOriginalSideLength(unsigned int index) {return original_side_lengths[index];}
};
