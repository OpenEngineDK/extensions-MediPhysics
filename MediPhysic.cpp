// Program name and description
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "MediPhysic.h"

#include "Mediotic/mediSurfaceCreationStrategy.h"
#include "Mediotic/mediObjSurfaceCreationStrategy.h"
#include "Mediotic/mediNeedleConstraintManipulator.h"
#include "Mediotic/mediIterativeConstraintManipulator.h"

#include <Logging/Logger.h>
#include <Renderers/OpenGL/TextureLoader.h>
#include <Resources/ResourceManager.h>
#include <Resources/ITextureResource.h>
#include <Scene/TransformationNode.h>

using namespace OpenEngine::Scene;
using namespace OpenEngine::Logging;
using namespace OpenEngine::Resources;
using namespace OpenEngine::Renderers::OpenGL;

MediPhysic::MediPhysic(string modeldir) {
    objdir = modeldir;
    needle = NULL;
    injected = false;
    transNode = NULL;
    initialized = false;
    primaryTextureID = 0;
    secondTextureID = 0;
}

MediPhysic::~MediPhysic() {}

/**
 * Returns a vector from Egg center to particle specified by the index
 * NOTE: no out of bounds check!
 */
Vector<3,float> MediPhysic::GetOrientationVector(int index) {

    mediParticle& p = mediObject->getParticles()[index];
    Vector<3,float> particle(p.getPos().x(), p.getPos().y(), p.getPos().z());

    Vector3 c = mediObject->getBoundingSphere().center;
    Vector<3,float> center(c.x(), c.y(), c.z());
    
    return Vector<3,float>(particle-center);
}

void MediPhysic::Handle(InitializeEventArg arg) {
    if (initialized) return;

    // Load the Egg
    mediObjSurfaceCreationStrategy* surfaceStrategy = 
        new mediObjSurfaceCreationStrategy(objdir + "SmallGeoSphere500.obj");
    mediObjSurfaceCreationStrategy* surfaceStrategy2 = 
        new mediObjSurfaceCreationStrategy(objdir + "BigGeoSphere500.obj");

    mediObject = new mediBody(NULL,surfaceStrategy);
    mediObject2 = new mediBody(NULL,surfaceStrategy2);
    surface = new RenderMediSurface(mediObject->getSurface());
    surface2 = new RenderMediSurface(mediObject2->getSurface());

    // Get loaded model to access texture coordinates (hack)
    surface->SetObjLoader(surfaceStrategy->GetObjModel());
    surface2->SetObjLoader(surfaceStrategy->GetObjModel());

    // add constraints to the objects them self
	mediObject->addForceManipulator
        (new mediIterativeConstraintManipulator(mediObject, 1, 1.0f));
	mediObject2->addForceManipulator
        (new mediIterativeConstraintManipulator(mediObject2, 1, 1.5f));

    // add constraints between the two objects
	mediObject->addForceManipulator
        (new mediIterativeConstraintManipulator(mediObject2, mediObject, 1, 1.0f));
	mediObject2->addForceManipulator
        (new mediIterativeConstraintManipulator(mediObject2,mediObject, 1, 1.0f));

    // save the state to be able to reset it later
    mediObject->saveState();
    mediObject2->saveState();

    Reset();

	mediObject->addForceManipulator
        (new mediNeedleConstraintManipulator(mediObject, needle,2.4));
	mediObject2->addConstraintManipulator
        (new mediNeedleConstraintManipulator(mediObject2, needle,3.1));
	
	ITextureResourcePtr tex = 
        ResourceManager<ITextureResource>::Create("EggTexture.tga");
    tex->Load();
    TextureLoader::LoadTextureResource(tex);
    primaryTextureID = tex->GetID();

    ITextureResourcePtr tex2 =
        ResourceManager<ITextureResource>::Create("EggTexture2.tga");
    tex2->Load();
    TextureLoader::LoadTextureResource(tex2);
    secondTextureID = tex2->GetID();
	
    injected = false;
    transNode = NULL;
    initialized = true;
}

void MediPhysic::Handle(ProcessEventArg arg) {
    if(!initialized) Handle(InitializeEventArg());

	mediObject2->applyForceManipulator();
	mediObject->applyForceManipulator();
			
	mediObject->applyTimeStep(0.03);
	mediObject2->applyTimeStep(0.03);
	
	mediObject->applyContraintManipulator();
	mediObject2->applyContraintManipulator();
}

/**
 * Draw the geometry, this is called by the renderer
 */
void MediPhysic::Apply(IRenderingView* rv) {
    surface->textureID = primaryTextureID;
    surface2->textureID = primaryTextureID;
    surface->draw();
	surface2->draw();

    surface->textureID = secondTextureID;
    surface2->textureID = secondTextureID;
    surface->draw();
	surface2->draw();
    
    if (injected) {
        //update spermatozoa.
        Vector3 eggPos(0,0,0);
        for (unsigned int i=0; i<mediObject->getParticles().size(); i++) {
            Vector3 pos = mediObject->getParticles()[i].getPos();
            eggPos += pos;
        }
        eggPos /= mediObject->getParticles().size();
        Vector3 dir = (eggPos + offset) - transNode->GetPosition();
        Vector3 newPos = transNode->GetPosition() + (dir * 0.1);
        
        if ((newPos-eggPos).length() > mediObject->getBoundingSphere().radius)
            logger.info << "outside of egg" << logger.end;
        transNode->SetPosition(Vector<3,float>(newPos[0],newPos[1],newPos[2]));
    }
}

void MediPhysic:: Handle(DeinitializeEventArg arg) {
    initialized = false;
}

bool MediPhysic::Inject(TransformationNode* trans) {
    if( injected ) return false; //return if already injected.

    Vector3 eggPos(0,0,0);
    for (unsigned int i=0; i<mediObject->getParticles().size(); i++) {
        Vector3 pos = mediObject->getParticles()[i].getPos();
        eggPos += pos;
    }
    eggPos /= mediObject->getParticles().size();
    Vector3 dist = trans->GetPosition() - eggPos;
    if (dist.length() < (mediObject->getBoundingSphere().radius * 0.8f)) {
        logger.info << "Injection successfully accomplished." << logger.end;
        injected = true;
        transNode = trans;
        offset = Vector<3,float>(dist[0],dist[1],dist[2]);
        return true;
    }
    logger.info << "Injection NOT successfull - try again!" << logger.end;
    return false;
}

void MediPhysic::Reset() {
    mediObject->loadState();
    mediObject2->loadState();
}
