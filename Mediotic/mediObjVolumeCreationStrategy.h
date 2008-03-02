#pragma once
#include "mediVolumeCreationStrategy.h"
#include "OBJLoader/ObjLoader.h"
#include <string>

class mediObjVolumeCreationStrategy : public mediVolumeCreationStrategy
{
private:
	ObjLoader obj;
	Vector3 resolutions;

public:
	mediObjVolumeCreationStrategy(void);
	~mediObjVolumeCreationStrategy(void);

	mediObjVolumeCreationStrategy(std::string filename,const Vector3 resolutions);
	void process(std::vector<mediParticle> & particles );

};
