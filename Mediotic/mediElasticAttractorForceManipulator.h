#pragma once
#include "mediManipulator.h"
#include "MathMisc.h"
#include "mediParticle.h"
#include "mediBody.h"

class mediElasticAttractorForceManipulator :
	public mediManipulator
{
	float forceweight;
	class Neighbourhood;
	vector<Neighbourhood> neighbours; // particle i has neighbourhood i.


	class Triple
	{
		mediParticle* particles[3];
		float weights[4]; //(alphai, alphaj, alphak, beta)
	
		void calculateWeights(Vector3 coord)
		{
				float abcArea = (cross((particles[0]->getPos()-particles[1]->getPos()),(particles[2]->getPos()-particles[1]->getPos()))).length();

				weights[0] = cross(particles[1]->getPos()-coord,particles[2]->getPos()-coord).length() / abcArea;
				weights[1] = cross(particles[0]->getPos()-coord,particles[2]->getPos()-coord).length() / abcArea;
				weights[2] = cross(particles[0]->getPos()-coord,particles[1]->getPos()-coord).length() / abcArea;
		}

	public:
		Triple() { weights[0]=0;weights[1]=0;weights[2]=0;weights[3]=0; }

		Triple(mediParticle *p1, mediParticle *p2, mediParticle *p3, mediParticle *reference)
		{
			particles[0]=p1;
			particles[1]=p2;
			particles[2]=p3;


			Vector3 coord = projectPointOntoPlane(particles[0]->getPos(),particles[1]->getPos(),particles[2]->getPos(),reference->getPos());
			weights[3] = (coord-reference->getPos()).length();


			// now length must have correct sign (not just positive)
			Vector3 vectorToReference = coord-reference->getPos();
			Vector3 normal = cross(particles[1]->getPos()-particles[0]->getPos(), particles[2]->getPos()-particles[0]->getPos());
			vectorToReference.makeUnitVector();
			normal.makeUnitVector();

			if (dot(vectorToReference,normal)>0) 
				weights[3]=-weights[3];

			calculateWeights(coord);

			/*if (isValid() && (getPoint()-reference->getPos()).length()>0.000001 )
			{
				std::cout << coord << std::endl;
				std:: cout << (particles[0]->getPos()*weights[0]+particles[1]->getPos()*weights[1]+particles[2]->getPos()*weights[2]) << std::endl;
				std::cout << (getPoint()-reference->getPos()) << std::endl;

			}*/

		}

		Vector3 getPoint()
		{

			Vector3 Qt = (particles[0]->getPos()*weights[0]+particles[1]->getPos()*weights[1]+particles[2]->getPos()*weights[2]); 
			Vector3 normal = cross(particles[1]->getPos()-particles[0]->getPos(), particles[2]->getPos()-particles[0]->getPos());

			normal.makeUnitVector();

			return Qt+weights[3]*normal;

		}

		bool isValid()
		{
			return (weights[0]+weights[1]+weights[2])<=1 && (weights[0]+weights[1]+weights[2])>0.9;
		}

	};

	class Neighbourhood
	{
	private:
		vector<Triple> triples;
		mediParticle* particle;
	public:
		Neighbourhood(vector<mediParticle>& all_particles, mediParticle* current_particle, float threshold)
		{
			particle = current_particle;
			vector<mediParticle *> close_particles;
//cout<< "sovs1" <<endl;

			for(unsigned int i = 0; i<all_particles.size(); i++)
			if (&all_particles[i] != current_particle && (all_particles[i].getPos()-current_particle->getPos()).length()<threshold )
			{
				close_particles.push_back(&all_particles[i]);
			}

//cout<< "sovs2" <<endl;

			//clean out in particles XXXX hack
			//if(false)
		/*	while (all_particles.size()>4)
			{
				int index = rand()%all_particles.size();
				all_particles.erase(all_particles.begin()+index);
			}*/
//cout<< "sovs3" <<endl;

			int stop = close_particles.size();

//			if (stop>10) stop = 10;
//			if (all_particles.size()>2)
//			for(int i=0; i<stop; i++)
//				for(int j=i+1; j<stop; j++)
//					for(int k=j+1; k<stop; k++)

			
				stop = 10000;
				int counter = 0;
				while(triples.size()<50 && counter<stop)
					{
						int ii = rand()%close_particles.size();
						int ji = rand()%close_particles.size();
						int ki = rand()%close_particles.size();
						Triple triple = Triple(close_particles[ii], close_particles[ji], close_particles[ki], current_particle);
						if (triple.isValid())
						{
							triples.push_back(triple);
							//cout << triples.size();
						}
						counter++;
					}

//cout<< "sovs4" <<endl;

		}

		void apply(float forceweight)
		{

			Vector3 p;
			for(unsigned int i=0; i < triples.size(); i++)
			{
				p = p + triples[i].getPoint();
			}
			p = p/triples.size();
			particle->addForce((p-particle->getPos())*forceweight);
		}

		bool isEmpty()
		{
			return triples.size()==0;
		}

		long getNoTriples()
		{
			return triples.size();

		}

	};

	

public:
	mediElasticAttractorForceManipulator(mediBody *body, float forceweight, float threshold);
	~mediElasticAttractorForceManipulator(void);
	void apply();
};
