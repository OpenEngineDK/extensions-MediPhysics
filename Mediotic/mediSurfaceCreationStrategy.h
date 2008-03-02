#pragma once

#include "mediParticle.h"
#include "mediSurface.h"
#include <vector>

class mediSurfaceCreationStrategy
{
public:
    virtual ~mediSurfaceCreationStrategy() {}
	virtual void process(std::vector<mediParticle> & particles, mediSurface & surface) = 0;

};
