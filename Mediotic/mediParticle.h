#pragma once
#include <vector>
#include "mediParticleTriple.h"
#include "mediVerletODESolver.h"
#include "mediFace.h"
#include "Vector2.h"
#include "Vector3.h"
#include "mediODESolver.h"

//class mediODESolver;

class mediParticle
{

private:
	std::vector<mediParticleTriple> triples;
	std::vector<mediFace *> faces;
	mediODESolver* solver;
	Vector3 position;
	Vector2 textureCoord;
	float inverse_mass;

public:
	mediParticle(void);
	mediParticle(Vector3 position, float invmass = 1);
    mediParticle(const mediParticle& copy);
	//mediParticle(Vector3 position, Vector2 textureCoord, float invmass = 1);
	~mediParticle(void);

	Vector3 &getPos() {return position;};
	void addFace(mediFace *face);
	Vector3 getNormal();

    //	void setPos(Vector3& pos) {position = pos;}
	void setPos(Vector3  pos) {position = pos;}
	void resetVelocity() {((mediVerletODESolver*) solver)->setOldPos(position);}
	void resetVelocityInYZ() {((mediVerletODESolver*) solver)->setOldPos(Vector3(((mediVerletODESolver*) solver)->getOldPos().x(),position.y(),position.z()));}

	void attachODESolver(mediODESolver solver); 
	void setForce(Vector3 force);
	void addForce(Vector3 force);
	void accelerate(Vector3 a);

	void attachODESolver(mediODESolver *solver);
	void timeStep(float stepsize);

	float getInvMass() {return inverse_mass;}
	bool isSurfaceParticle() {return faces.size()>0;}
};

