#pragma once
#include "mediVolumeCreationStrategy.h"
#include "Vector3.h"
#include "mediParticle.h"

class mediCubeVolumeCreationStrategy :
	public mediVolumeCreationStrategy
{
private:
	Vector3 center;
	Vector3 dimensions;
	Vector3 resolutions;

public:
	mediCubeVolumeCreationStrategy(const Vector3 center,const Vector3 dimensions,const Vector3 resolutions);
	~mediCubeVolumeCreationStrategy(void);

	void process(std::vector<mediParticle> & particles );
};
