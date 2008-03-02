#pragma once
#include "mediParticle.h"
#include "mediSurfaceCreationStrategy.h"
#include "mediVolumeCreationStrategy.h"
#include "mediNeighbourhoodCreationStrategy.h"
#include "mediManipulator.h"
#include "BVH/BVHBoundingSphere.h"

class mediBoundingSphere
{
public:
	float radius;
	Vector3 center;
	mediBoundingSphere(float radius, Vector3 center) : radius(radius),center(center) {};
	mediBoundingSphere() {};
};

class mediBody
{
protected:
	std::vector<mediParticle> particles;
	std::vector<mediParticle> initParticles; //snapshot of all particles

	mediSurface surface;
	std::vector<mediManipulator*> forceManipulators;
	std::vector<mediManipulator*> constraintManipulators;

public:
	mediBody(mediVolumeCreationStrategy *v,mediSurfaceCreationStrategy *s);
	~mediBody(void);

	void addForceManipulator( mediManipulator *manipulator ) { forceManipulators.push_back(manipulator); }
	void addConstraintManipulator( mediManipulator *manipulator) {constraintManipulators.push_back(manipulator);};

	mediSurface& getSurface() {return surface;};
	std::vector<mediParticle>& getParticles() { return particles; };

	mediBoundingSphere getBoundingSphere();

	void timeStep(float stepsize);
	void applyForceManipulator();
	void applyTimeStep(float stepsize);
	void applyContraintManipulator();

    void saveState();
    void loadState();
};
