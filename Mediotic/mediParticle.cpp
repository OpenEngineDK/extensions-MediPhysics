#include "mediParticle.h"
#include "mediODESolver.h"
#include "mediVerletODESolver.h"

mediParticle::mediParticle(void)
: position(Vector3(0,0,0)), inverse_mass(1.0f)
{
	//this->attachODESolver(new mediVerletODESolver(0.1));
}

mediParticle::mediParticle(Vector3 position, float mass)
: position(position), inverse_mass(1.0f/mass)
{
	//this->attachODESolver(new mediVerletODESolver(0.1));
}

/*
mediParticle::mediParticle(Vector3 position, Vector2 textureCoord, float mass)
: position(position), inverse_mass(1.0f/mass) {
  this->textureCoord = textureCoord;
	//this->attachODESolver(new mediVerletODESolver(0.1));
}
*/

mediParticle::mediParticle(const mediParticle& copy){
    triples.clear();
    for(unsigned int j=0; j<copy.triples.size(); j++){
        triples.push_back(copy.triples[j]);
    }
    
    faces.clear();
    for(unsigned int i=0; i<copy.faces.size(); i++){
        faces.push_back(copy.faces[i]);
    }
	
    this->solver = copy.solver;
    this->position = Vector3(copy.position);
    this->textureCoord = Vector2(copy.textureCoord);
	this->inverse_mass = copy.inverse_mass;

}


mediParticle::~mediParticle(void)
{
}

void mediParticle::addFace(mediFace *face)
{
	faces.push_back(face);
}

Vector3 mediParticle::getNormal()
{
	int count = 0;
	Vector3 res(0,0,0);
	for (unsigned int i = 0; i<faces.size(); i++)
	{
		res = res + faces[i]->getNormal();

		count ++;	
	}
	res.makeUnitVector();
	return res;
}

void mediParticle::attachODESolver(mediODESolver *_solver) 
{
	this->solver = _solver;
	this->solver->setParticle(this);
}

void mediParticle::timeStep(float stepsize)
{
	this->solver->timeStep(stepsize);
	this->setForce(Vector3(0,0,0));
}

void mediParticle::setForce(Vector3 force)
{
	solver->setForce(force); 

};
void mediParticle::addForce(Vector3 force) {solver->addForce(force); };
void mediParticle::accelerate(Vector3 a) {solver->accelerate(a); };
