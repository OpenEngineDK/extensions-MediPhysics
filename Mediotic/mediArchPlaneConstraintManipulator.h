#pragma once
#include "mediManipulator.h"
#include "Vector3.h"
#include "mediBody.h"

using std::vector;

class mediArchPlaneConstraintManipulator :	public mediManipulator
{
private:
	mediBody* body;             // body of particles
    Vector3   body_sphere;      // center of body sphere
    float     body_radius;
	Vector3 c;                  // center of selection sphere
	float   sr;                 // squared radius of selection sphere
    vector<float> zs;           // initial z values of particles

    float scaling(Vector3 pos) {
        float d = (c - pos).squaredLength();
        if (d >= sr) return 0;
        return 1 - (d / sr);
    }

public:
	mediArchPlaneConstraintManipulator(mediBody* body,
                                       float sphere_radius);

	virtual ~mediArchPlaneConstraintManipulator(void);
    
	void apply();
    void draw();

};
