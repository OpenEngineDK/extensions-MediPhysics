#include "mediParticle.h"
#include <vector>

class mediNeighbourhoodCreationStrategy
{
public:
    virtual ~mediNeighbourhoodCreationStrategy(){}
	virtual void process(std::vector<mediParticle> & particles ) = 0;

};
