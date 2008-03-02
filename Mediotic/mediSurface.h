#pragma once
#include <vector>
#include "mediFace.h"
class mediBody;

class mediSurface
{
private:
	std::vector<mediFace> faces;
	mediBody *body;

public:
	mediSurface(void) ;
	~mediSurface(void);
	void reserveNumberOfFaces(unsigned int n) { faces.reserve(n); };
	void addFace(mediFace &f) {faces.push_back(f);};
	void addFace(mediParticle *p1, mediParticle *p2, mediParticle *p3) {faces.push_back(mediFace(p1,p2,p3));};
	std::vector<mediFace>& getFaces() {return faces;};
	
	void setBody(mediBody *body) {this->body = body;};
	std::vector<mediParticle>& getParticles();
	float getVolume();

};
