
ADD_LIBRARY(Extensions_MediPhysics
  MediPhysic.cpp
)

TARGET_LINK_LIBRARIES(Extensions_MediPhysics
  Mediotic
  ${GLUT_LIBRARIES}
)
