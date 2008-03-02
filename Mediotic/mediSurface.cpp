#include "mediSurface.h"
#include "mediBody.h"
#include "math.h"
#include "mediBody.h"

mediSurface::mediSurface(void)
{

}

mediSurface::~mediSurface(void)
{
}

float mediSurface::getVolume(void)
{
	const double max_tetrahedon_volume = 10000000;
	//can be optimized when vector is origo
	const Vector3 reference_point(200,200,200); 

	std::vector<mediFace>& faces = getFaces();

	double total_volume = 0.0;

	std::vector<mediFace>::iterator it;
	for(it = faces.begin(); it!=faces.end(); it++)
	{
		mediFace& face = *it;

		Vector3 a = face[0].getPos();
		Vector3 b = face[1].getPos();		
		Vector3 c = face[2].getPos();

		double sign = ( dot(face.getNormal() , a - reference_point)>0 ? 1.0 : -1.0);

		double tetrahedon_volume = (double) fabs( dot( ( reference_point - a), cross(reference_point - b, reference_point - c) ) ) / 6.0;

		if (tetrahedon_volume<max_tetrahedon_volume && tetrahedon_volume>0)	
			total_volume += sign*tetrahedon_volume;
		
	}

	return (float) total_volume;
}

std::vector<mediParticle>& mediSurface::getParticles() {
    return body->getParticles();
};
