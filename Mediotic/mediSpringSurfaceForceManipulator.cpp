#include "mediSpringSurfaceForceManipulator.h"
#include "mediSurface.h"
#include "mediFace.h"

mediSpringSurfaceForceManipulator::mediSpringSurfaceForceManipulator(mediBody *body, float spring_constant) 
: body(body), spring_constant(spring_constant)
{
}

mediSpringSurfaceForceManipulator::~mediSpringSurfaceForceManipulator(void)
{
}

void mediSpringSurfaceForceManipulator::apply()
{
	mediSurface & surface = body->getSurface();

	std::vector<mediFace>& faces = surface.getFaces();

	Vector3 actual_side_lengths;
	Vector3 side1;
	Vector3 side2;
	Vector3 side3;

	std::vector<mediFace>::iterator it;
	for(it = faces.begin(); it!=faces.end(); it++)
	{
		mediFace& face = *it;

		Vector3 pos1 = face[0].getPos();
		Vector3 pos2 = face[1].getPos();		
		Vector3 pos3 = face[2].getPos();

		side1 = (pos2 - pos1);
		side2 = (pos3 - pos2);
		side3 = (pos1 - pos3);

		actual_side_lengths[0] = side1.length();
		actual_side_lengths[1] = side2.length();
		actual_side_lengths[2] = side3.length();

		// first side in face
		float length_difference = actual_side_lengths[0] - face.getOriginalSideLength(0); 
		face[0].addForce(spring_constant*length_difference*side1);
		face[1].addForce(-spring_constant*length_difference*side1);

		// second side in face
		length_difference = actual_side_lengths[1] - face.getOriginalSideLength(1); 
		face[1].addForce(spring_constant*length_difference*side2);
		face[2].addForce(-spring_constant*length_difference*side2);

		// third side in face
		length_difference = actual_side_lengths[2] - face.getOriginalSideLength(2); 
		face[2].addForce(spring_constant*length_difference*side3);
		face[0].addForce(-spring_constant*length_difference*side3);

	}

}
