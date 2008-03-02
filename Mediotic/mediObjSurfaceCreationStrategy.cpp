#include "OBJLoader/ObjLoader.h"
#include "mediObjSurfaceCreationStrategy.h"
#include "mediVerletODESolver.h"

mediObjSurfaceCreationStrategy::mediObjSurfaceCreationStrategy(void)
{
}

mediObjSurfaceCreationStrategy::~mediObjSurfaceCreationStrategy(void)
{
}


mediObjSurfaceCreationStrategy::mediObjSurfaceCreationStrategy(std::string filename)
{
	obj = ObjLoader(filename.c_str());
    //    obj.scaleVertices(10.0f);
	//obj.grow(1);
}

void mediObjSurfaceCreationStrategy::process(std::vector<mediParticle> & particles, mediSurface & surface)
{

	particles.reserve(particles.size()+obj.v.size()*2);
	int startIndex = (int) particles.size();

	for(unsigned int i=0; i<obj.v.size(); i++)
	{
		particles.push_back(mediParticle(obj.v[i]));
	}

	surface.reserveNumberOfFaces((int) obj.f.size()*2);

	for(unsigned int i=0; i<obj.f.size(); i++)
    {
		surface.addFace(&particles[obj.f[i].v[0]-1+startIndex],&particles[obj.f[i].v[1]-1+startIndex],&particles[obj.f[i].v[2]-1+startIndex] );
		particles[obj.f[i].v[0]-1+startIndex].addFace(&surface.getFaces()[surface.getFaces().size()-1]);
		particles[obj.f[i].v[1]-1+startIndex].addFace(&surface.getFaces()[surface.getFaces().size()-1]);
		particles[obj.f[i].v[2]-1+startIndex].addFace(&surface.getFaces()[surface.getFaces().size()-1]);
	}

}

ObjLoader* mediObjSurfaceCreationStrategy::GetObjModel(){
    return &obj;
}
