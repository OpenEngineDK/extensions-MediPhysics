ADD_LIBRARY(BVH
	    BVHBoundingBox.cpp
	    BVHTriangle.cpp
	    BVHBoundingSphere.cpp
	    BVHRay.cpp )

TARGET_LINK_LIBRARIES(BVH
		      Mediotic)
