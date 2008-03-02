#include "OBJLoader/ObjLoader.h"
#include "mediObjVolumeCreationStrategy.h"
#include "BVH/bvh.h"
#include "BVH/BVHRay.h"
#include "BVH/BVHBoundingSphere.h"
#include "BVH/BVHTriangle.h"

#include "mediVerletODESolver.h"

mediObjVolumeCreationStrategy::mediObjVolumeCreationStrategy(void)
{
}

mediObjVolumeCreationStrategy::~mediObjVolumeCreationStrategy(void)
{
}


mediObjVolumeCreationStrategy::mediObjVolumeCreationStrategy(std::string filename,const Vector3 resolutions)
: resolutions(resolutions)
{
    obj = ObjLoader(filename.c_str());
}

void mediObjVolumeCreationStrategy::process(std::vector<mediParticle> & particles )
{
   	BVH<BVHBoundingSphere> bvh = BVH<BVHBoundingSphere>();
	for(unsigned int i=0; i<obj.f.size(); i++)
	{
		bvh.insert( BVHTriangle(0, obj.v[obj.f[i].v[0]-1], obj.v[obj.f[i].v[1]-1], obj.v[obj.f[i].v[2]-1]));
	}
	bvh.buildHierarchy();
	std::cout << "BVH hierarchy build" << std::endl;

	Vector3 minV, maxV;
	obj.boundingBox(minV,maxV);

	/*for(int x=0; x<resolutions[0]; x++)
		for( int y=0; y<resolutions[1]; y++)
			for( int z=0; z<resolutions[2]; z++)
			{
				Vector3 v( (1-(float(x)/resolutions[0]))*minV[0]+(float(x)/resolutions[0])*maxV[0],
					(1-(float(y)/resolutions[1]))*minV[1]+(float(y)/resolutions[1])*maxV[1],
					(1-(float(z)/resolutions[2]))*minV[2]+(float(z)/resolutions[2])*maxV[2]);

				particles.push_back(mediParticle(v));
		}*/

	for(int x=0; x<resolutions[0]; x++)
		for( int y=0; y<resolutions[1]; y++)
		{
			
				Vector3 orig( (1-(float(x)/resolutions[0]))*minV[0]+(float(x)/resolutions[0])*maxV[0],
					(1-(float(y)/resolutions[1]))*minV[1]+(float(y)/resolutions[1])*maxV[1],
					minV[2]);

			Vector3 dir = Vector3(0,0,1);

			float EPS=0.01;
			

			BVHTriangleHit hit1 = bvh.rayIntersection(BVHRay(orig,dir));
			while(hit1.distance!=std::numeric_limits<float>::infinity())
			{
				particles.push_back(mediParticle(orig+ (dir*hit1.distance)));

				BVHTriangleHit hit2 = bvh.rayIntersection(BVHRay(orig+dir*(hit1.distance+EPS),dir));
				if (hit2.distance!=std::numeric_limits<float>::infinity())
				{
					particles.push_back(mediParticle(orig+ dir*(hit1.distance+EPS) + dir*(hit2.distance)));
					
					float d =0;
					while( d<hit2.distance)
					{
						d=d + (maxV[2]-minV[2])/resolutions[2];
						particles.push_back(mediParticle(orig + dir*(hit1.distance+EPS+d) ));
					}
				}
				else
				{
					std::cerr << "Geometry not closed" << std::endl;
					//exit(0);

				}

				orig = orig + dir*(hit1.distance+EPS) + dir*(hit2.distance+EPS); // new orig at end of solid part
				hit1 = bvh.rayIntersection(BVHRay(orig,dir));
			}

		}

}
