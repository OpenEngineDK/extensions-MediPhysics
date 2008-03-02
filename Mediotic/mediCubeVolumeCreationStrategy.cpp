#include "mediCubeVolumeCreationStrategy.h"

mediCubeVolumeCreationStrategy::mediCubeVolumeCreationStrategy(const Vector3 center,const Vector3 dimensions,const Vector3 resolutions)
: center(center), dimensions(dimensions), resolutions(resolutions)
{
}

mediCubeVolumeCreationStrategy::~mediCubeVolumeCreationStrategy(void)
{
}

void mediCubeVolumeCreationStrategy::process(std::vector<mediParticle> & particles )
{
	Vector3 corner = center - dimensions*0.5;

	for(int x=0; x<resolutions[0]; x++)
		for(int y=0; y<resolutions[1]; y++)
			for( int z=0; z<resolutions[2]; z++)
				particles.push_back(mediParticle(corner + (dimensions /resolutions)*Vector3(x,y,z)));
    
}


