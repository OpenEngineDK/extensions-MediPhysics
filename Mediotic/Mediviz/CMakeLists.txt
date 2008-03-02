
ADD_LIBRARY(Mediviz
	    ArcBall.cpp
	    RenderMediSurface.cpp )

TARGET_LINK_LIBRARIES(Mediviz
		      Mediotic 
 		     ${GLEW_LIBRARY}   )

