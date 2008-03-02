#pragma once

#include "mediParticle.h"
#include <vector>

class mediVolumeCreationStrategy
{
public:
    virtual ~mediVolumeCreationStrategy() {}

	virtual void process(std::vector<mediParticle> & particles ) = 0;

};
