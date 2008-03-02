#pragma once
#include "mediSurfaceCreationStrategy.h"
#include "OBJLoader/ObjLoader.h"

#include <string>
class mediObjSurfaceCreationStrategy : public mediSurfaceCreationStrategy
{
private:
	ObjLoader obj;

public:
	mediObjSurfaceCreationStrategy(void);
	~mediObjSurfaceCreationStrategy(void);

	mediObjSurfaceCreationStrategy(std::string filename);
	void process(std::vector<mediParticle> & particles, mediSurface & surface);

    ObjLoader* GetObjModel();
    void CopyObjModel(ObjLoader obj);
};
