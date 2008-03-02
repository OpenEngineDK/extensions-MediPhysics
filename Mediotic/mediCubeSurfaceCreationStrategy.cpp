#include "mediCubeSurfaceCreationStrategy.h"
#include <map>

mediCubeSurfaceCreationStrategy::mediCubeSurfaceCreationStrategy(const Vector3 center,const Vector3 dimensions,const Vector3 resolutions)
: center(center), dimensions(dimensions), resolutions(resolutions)
{
}


mediCubeSurfaceCreationStrategy::~mediCubeSurfaceCreationStrategy(void)
{
}


void mediCubeSurfaceCreationStrategy::process(std::vector<mediParticle> & particles, mediSurface & surface )
{
	std::map<Vector3,mediParticle*> edgeMap;

	Vector3 corner = center - dimensions*0.5;

	for(int x=0; x<resolutions[0]; x++)
	  for(int y=0; y<resolutions[1]; y++)
	    for( int z=0; z<resolutions[2]; z++)
	      {
		// only make particles on the faces  
		if (x!=0 && x!=(resolutions[0]-1))
		  continue;
		if (y!=0 && y!=(resolutions[1]-1))
		  continue;
		if (z!=0 && z!=(resolutions[2]-1))
		  continue;
		
		Vector3 position(x,y,z);
		
		std::map<Vector3,mediParticle*>::iterator iter = edgeMap.find(position);

		if( iter != edgeMap.end() ) {
		  // Particle already exists
		}
		else
		  {
		    particles.push_back(mediParticle(corner + (dimensions /resolutions)*position));
		    edgeMap[position] = &particles[particles.size()-1];
		  }
		
	      }
	
		for(int x=1; x<resolutions[0]; x++)
			for(int y=1; y<resolutions[1]; y++)
			{
				int z=0;
				Vector3 position(x,y,z);
				std::map<Vector3,mediParticle*>::iterator iter1,iter2,iter3, iter4;

				iter4 = edgeMap.find(position);

				if (iter4==edgeMap.end()) 
					continue;

				iter1 = edgeMap.find(position-Vector3(1,0,0));
				iter2 = edgeMap.find(position-Vector3(0,1,0));
				iter3 = edgeMap.find(position-Vector3(1,1,0));

				if (iter1!=edgeMap.end() && iter2!=edgeMap.end() && iter3!=edgeMap.end())
				{
					mediParticle* here = iter4->second;  
					mediParticle* prev1 = iter1->second;  
					mediParticle* prev2 = iter2->second;  
					mediParticle* prevboth = iter3->second;  

					// create two triangles and associate with "here"-particle
					surface.addFace(here,prevboth,prev1);
					surface.addFace(here,prev2,prevboth);

				}

			}


		
		
}
