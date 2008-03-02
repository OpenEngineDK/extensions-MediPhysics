#include "mediPressureSurfaceForceManipulator.h"
#include "mediSurface.h"
#include "mediFace.h"

mediPressureSurfaceForceManipulator::mediPressureSurfaceForceManipulator(mediBody *body, float reaction_factor) {
  this->body = body;
  this->reaction_factor = reaction_factor;
  desired_volume = body->getSurface().getVolume();
}

mediPressureSurfaceForceManipulator::~mediPressureSurfaceForceManipulator(void)
{
}

void mediPressureSurfaceForceManipulator::apply() {
  //const float minimum_face_area = 0.001;
  //const float maximum_face_area = 100.00;

	mediSurface & surface = body->getSurface();

	float actual_volume = surface.getVolume();
	float volume_difference = actual_volume - desired_volume;

	//std::cout << actual_volume << "/" << desired_volume << std::endl;

	std::vector<mediFace>& faces = surface.getFaces();

	for(unsigned int i = 0; i<faces.size(); i++)
	{ 
		mediFace& face = faces[i];

		Vector3 normal = face.getNormal();

		float face_area = cross(face[2].getPos()-face[0].getPos() , face[1].getPos()-face[0].getPos() ).length(); // ( * 0.5 )!

		//if (face_area>minimum_face_area && face_area<maximum_face_area)

		/*volume_difference = (volume_difference<-1 ? -1 : volume_difference ); 
		volume_difference = (volume_difference> 1 ?  1 : volume_difference ); 
*/
		{
			Vector3 normal_force =  - face_area * reaction_factor * volume_difference * normal;

			face[0].addForce(normal_force);
			face[1].addForce(normal_force);
			face[2].addForce(normal_force);
		}
	}

}
