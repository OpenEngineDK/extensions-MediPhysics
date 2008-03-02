#include "mediElasticAttractorForceManipulator.h"

mediElasticAttractorForceManipulator::mediElasticAttractorForceManipulator(mediBody *body, float forceweight, float threshold) : forceweight(forceweight)
{
	{
		vector<mediParticle>& all_particles = body->getParticles();
		for(unsigned int i=0; i<all_particles.size(); i++)
		{
			Neighbourhood neighbourhood = Neighbourhood(all_particles, &all_particles[i], threshold);
			if (!neighbourhood.isEmpty())
			{
				neighbours.push_back(neighbourhood);
		//	std::cout << "juhu" << std::endl;
			}	
			std::cout << (float(i)/all_particles.size())*100 << "%" << std::endl;
		}
		
	};

}

mediElasticAttractorForceManipulator::~mediElasticAttractorForceManipulator(void)
{
}

void mediElasticAttractorForceManipulator::apply()
{
	for (unsigned int i=0; i<neighbours.size(); i++)
	{
		neighbours[i].apply(forceweight);
	}
}
