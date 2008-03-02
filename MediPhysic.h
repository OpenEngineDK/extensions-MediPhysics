// Program name and description
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _MEDI_PHYSIC_H_
#define _MEDI_PHYSIC_H_

#include <Core/IModule.h>
#include <Scene/ISceneNode.h>
#include <Scene/ISceneNodeVisitor.h>
#include <Scene/GeometryNode.h>
#include <Renderers/IRenderNode.h>
#include <list>
#include <string>
#include "Mediotic/mediBody.h"	
#include "Mediotic/Mediviz/RenderMediSurface.h"
#include "Mediotic/mediArchPlaneConstraintManipulator.h"

using OpenEngine::Core::IModule;

using namespace OpenEngine::Geometry;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Renderers;
using namespace std;

/**
 * MediPhysic module
 *
 * @class MediPhysic MediPhysic.h
 */
class MediPhysic : public IModule, public IRenderNode {
private:
    string objdir;

    mediBody* mediObject;
    mediBody* mediObject2;

    RenderMediSurface* surface;
	RenderMediSurface* surface2;

    mediArchPlaneConstraintManipulator* constraint1;
    mediArchPlaneConstraintManipulator* constraint2;

    bool injected, initialized;
    Vector<3,float> offset;
    int primaryTextureID;
    int secondTextureID;

public:
    MediPhysic(string modeldir);
    ~MediPhysic();

    void Initialize();
    void Process(const float deltaTime, const float percent);
    void Deinitialize();
    bool IsTypeOf(const std::type_info& inf);

    Vector<3,float> GetOrientationVector(int index);

    void Reset();
    bool Inject(TransformationNode* trans);

    virtual void Apply(IRenderingView* rv);
    //TEMP
    TransformationNode* needle;
    TransformationNode* transNode;
};

#endif
