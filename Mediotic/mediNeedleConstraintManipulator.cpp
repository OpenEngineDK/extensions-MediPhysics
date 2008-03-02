#include "mediNeedleConstraintManipulator.h"
#include "mediIterativeConstraintManipulator.h"
#include <Meta/OpenGL.h>
#include "../Meta/GLUT.h"
#include <Logging/Logger.h>

mediNeedleConstraintManipulator::mediNeedleConstraintManipulator(mediBody* body, TransformationNode* needle, float radius) : body(body), needle(needle), radius(radius) {
    oldPos = Vector3( needle->GetPosition() );
	needleState = AIR;

	onNeedle.attachODESolver(new mediVerletODESolver(0.01));
}

mediNeedleConstraintManipulator::~mediNeedleConstraintManipulator(void) {
}

void mediNeedleConstraintManipulator::apply(void) {
  applySphereCenter();
   applyLine();
}

Vector<3,float> mediNeedleConstraintManipulator::GetPos(mediParticle *p) {
    return Vector<3,float>(p->getPos().x(), p->getPos().y(), p->getPos().z());
}

void mediNeedleConstraintManipulator::applyLine(void) {
	float friction = 5;
	float threshold = 1;

	//cout << onNeedle.getPos().length() << " " << needle->GetPosition().GetLength() << endl;

	if ((onNeedle.getPos().length()+0.3)<needle->GetPosition().GetLength() && needleState==PENETRATED || 
        (needle->GetPosition().GetLength() > radius+0.3))
	{
		needleState=AIR;
        //		cout << onNeedle.getPos().length() << " " << needle->GetPosition().GetLength() << endl;
		springs.clear();
	}
	else if (needle->GetPosition().GetLength()<radius && needleState==AIR) // første penetration, opsamling af overflade-punkter
	{
		//Setup the "onNeedle" point
		onNeedle.setPos(needle->GetPosition());
		onNeedle.resetVelocity();
		oldPos = needle->GetPosition();

		// collect nearest neighbours, create springs, and put into springs-vector
		vector<mediParticle>::iterator it;

		for(unsigned int i=0 ; i<body->getParticles().size();i++)
			if ((body->getParticles()[i].getPos()-onNeedle.getPos()).length()<threshold )
			{
				mediParticle *p1 = &(body->getParticles()[i]);

				Vector3 vec = (p1->getPos()-onNeedle.getPos());

				springs.push_back(mediIterativeConstraintManipulator::Constraint(p1,&onNeedle));
			}
	    needleState = SURFACE;
	}
	else if (needleState==SURFACE)
	{
		float sum = 0.0;
		for(vector<mediIterativeConstraintManipulator::Constraint>::iterator it = springs.begin(); it != springs.end(); it++)
		{
			sum += (*it).apply();
		}
        
		onNeedle.setPos(needle->GetPosition());
		onNeedle.resetVelocity();

		if (sum>0.5) 
		{
			needleState=PENETRATED;
		}
	}
	else if (needleState==PENETRATED) // er penetreret, overfladen skal nu følge med
	{
		// Invariant: onNeedle.y == oldPos.y
		// Der er muligvis tilføjet kræfter fra andre fjedre.
	
		for(vector<mediIterativeConstraintManipulator::Constraint>::iterator it = springs.begin(); it != springs.end(); it++)
		{
			(*it).apply();
		}

		Vector3 newPos = needle->GetPosition();

		// kræft i x-retning, baseret på nålens bevægelse i x-retning.
		Vector3 needleForce = (newPos-oldPos)*friction; 

		needleForce.setY(0);
		needleForce.setZ(0);

		// tilføjer kræften fra nålen i x-retning.

		onNeedle.setForce(needleForce);
		onNeedle.timeStep(0.5);

		// sætter positionen for onNeedle til at være på (i y) nålen.
		Vector3 p = onNeedle.getPos(); // denne er muligvis ikke på nålen (i y)
		p.setY(newPos.y());
		onNeedle.setPos(p);
		onNeedle.resetVelocityInYZ();
				
		oldPos=newPos;
		// Invariant: onNeedle.y == newPos.y, og ingen velocity i YZ
		/*
		glPushMatrix();
		glTranslatef(p[0], p[1], p[2]);
		glutWireSphere(0.1,10,10);
		glPopMatrix();
		*/
	}
}

void mediNeedleConstraintManipulator::applySphere(void) {
    Vector<3,float> pos = needle->GetPosition();
    float size = 1.0f;

    Vector3 p = onNeedle.getPos();

    /*
    // Draw penetrating sphere for debug purpose
    glPushMatrix();
    glTranslatef(p[0], p[1], p[2]);
    glutWireSphere(size,10,10);
    glPopMatrix();
    */

    mediBody* mediObject = body;
    vector<mediParticle>& particles = mediObject->getParticles();
    vector<mediParticle>::iterator itr;
    for( itr=particles.begin(); itr!=particles.end(); itr++ ){
        mediParticle& p = *itr;
        Vector<3,float> particlePos(p.getPos().x(), p.getPos().y(), p.getPos().z());
        Vector<3,float> dir = particlePos - pos;
        
        if( dir.GetLength() < size ){
            float offset = size - dir.GetLength();
            //offset *= 4; //increase the offset 
            Vector<3,float> newPos = particlePos + (dir * offset);
            p.setPos(Vector3(newPos[0], newPos[1], newPos[2]));
	    logger.info << "Offset: " << offset << " dir: " << dir << logger.end;
			//p.resetVelocity(); //why?
        }
    }
}


/**
 * Center the egg so it stays within the scene.
 *
 */
void mediNeedleConstraintManipulator::applySphereCenter(void) {
  Vector<3,float> center(0, 0, 0);

  mediBody* mediObject = body;
  vector<mediParticle>& particles = mediObject->getParticles();
  vector<mediParticle>::iterator itr;
  for( itr=particles.begin(); itr!=particles.end(); itr++ ){
    mediParticle& p = *itr;
    Vector<3,float> particlePos(p.getPos().x(), p.getPos().y(), p.getPos().z());
    Vector<3,float> dir = center - particlePos;
    float force = dir.GetLength() * 0.0002;
    dir.Normalize();
    Vector<3,float> newPos = particlePos + (dir * force);
    p.setPos(Vector3(newPos[0], newPos[1], newPos[2]));
  }
}
