SUBDIRS( BVH
	  Mediviz
	  OBJLoader )

ADD_LIBRARY(Mediotic
	    MathMisc.cpp
	    mediElasticAttractorForceManipulator.cpp
	    mediObjVolumeCreationStrategy.cpp
            mediSpringAllForceManipulator.cpp
	    mediAxisAlignedBoxConstraintManipulator.cpp
	    mediArchPlaneConstraintManipulator.cpp
	    mediNeedleConstraintManipulator.cpp
	    mediFace.cpp
	    mediODESolver.cpp
            mediSpringSurfaceForceManipulator.cpp
	    mediAxixAlignedBoxConstraintManipulator.cpp
	    mediGravityForceManipulator.cpp
            mediParticle.cpp
            mediSurface.cpp
	    mediBody.cpp
            mediIterativeConstraintManipulator.cpp
	    mediParticleTriple.cpp
            mediVerletODESolver.cpp
	    mediCubeSurfaceCreationStrategy.cpp
	    mediManipulator.cpp
            mediPlasticSpringAllForceManipulator.cpp
	    mediCubeVolumeCreationStrategy.cpp
            mediObjSurfaceCreationStrategy.cpp
            mediPressureSurfaceForceManipulator.cpp
	    timeexp.cpp )

TARGET_LINK_LIBRARIES(  Mediotic
			Mediviz
			OBJLoader 
)
