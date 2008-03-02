#pragma once
#include "mediSurfaceCreationStrategy.h"
#include "Vector3.h"


class mediCubeSurfaceCreationStrategy :	public mediSurfaceCreationStrategy
{
private:
	Vector3 center;
	Vector3 dimensions;
	Vector3 resolutions;

public:
	mediCubeSurfaceCreationStrategy(const Vector3 center,const Vector3 dimensions,const Vector3 resolutions);
	~mediCubeSurfaceCreationStrategy(void);

	void process(std::vector<mediParticle> & particles, mediSurface & surface  );
};
