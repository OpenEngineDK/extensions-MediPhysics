#include "MathMisc.h"



Vector3 projectPointOntoPlane(Vector3 planeP1, Vector3 planeP2, Vector3 planeP3, Vector3 point)
{
			Vector3 a = planeP1-planeP2;
			Vector3 b = planeP1-planeP3;

			Vector3 normal;
			normal = cross(a,b);
			normal.makeUnitVector();
			Vector3 c = point-planeP1;
			return point + normal*-dot(c,normal);

}
