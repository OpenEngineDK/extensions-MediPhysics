#pragma once
#include "mediManipulator.h"
#include "Vector3.h"
#include "mediBody.h"
#include <Scene/TransformationNode.h>
#include <Math/Vector.h>
#include "mediIterativeConstraintManipulator.h"

using OpenEngine::Scene::TransformationNode;
using OpenEngine::Math::Vector;

enum NeedleState{AIR,SURFACE,PENETRATED};

class mediNeedleConstraintManipulator :	public mediManipulator
{
	
private:
	mediBody* body;
    TransformationNode* needle;
	float radius;
    void applyLine(void);
    void applySphere(void);
    void applySphereCenter(void);

    Vector<3,float> GetPos(mediParticle *p);
    Vector3 oldPos;
	vector<mediIterativeConstraintManipulator::Constraint> springs;
	mediParticle onNeedle;
	NeedleState needleState;


public:
	mediNeedleConstraintManipulator(mediBody* body, TransformationNode* needle,float radius);
	virtual ~mediNeedleConstraintManipulator(void);
	void apply();
};
