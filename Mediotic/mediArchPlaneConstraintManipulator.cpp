#include "mediArchPlaneConstraintManipulator.h"
#include <Meta/OpenGL.h>
#include "../Meta/GLUT.h"

mediArchPlaneConstraintManipulator::mediArchPlaneConstraintManipulator(mediBody* body,
                                                                       float sphere_radius)
    : body(body) {

    sr = sphere_radius * sphere_radius;
    vector<mediParticle>& particles = body->getParticles();
	vector<mediParticle>::iterator itr;
    Vector3 max = particles.begin()->getPos();
    Vector3 min = max;

    for (itr = particles.begin(); itr != particles.end(); itr++) {
        Vector3 v = (*itr).getPos();
        // save the z position
        zs.push_back(v.z());
        // find min and max
        if (v[0] < min[0]) min[0] = v[0];
        if (v[1] < min[1]) min[1] = v[1];
        if (v[2] < min[2]) min[2] = v[2];
        if (v[0] > max[0]) max[0] = v[0];
        if (v[1] > max[1]) max[1] = v[1];
        if (v[2] > max[2]) max[2] = v[2];
        if (v[2] > 5) cout << v << endl;
    }
    // save the body center and radius
    body_sphere = Vector3(max[0]+min[0], max[1]+min[1], max[2]+min[2]);
    body_radius = (particles.begin()->getPos() - body_sphere).length();
    cout << body_sphere << endl;
    cout << body_radius << endl;
    // save the constraint center
    c = Vector3(min[0], max[1]+min[1], max[2]+min[2]);
}

mediArchPlaneConstraintManipulator::~mediArchPlaneConstraintManipulator(void) {}

void mediArchPlaneConstraintManipulator::apply(void) {
	vector<mediParticle>& particles = body->getParticles();
	//vector<mediParticle>::iterator it;
	//for(it = particles.begin(); it!=particles.end(); it++) {
    for (unsigned int i=0; i<particles.size(); i++) {
        Vector3 pos = particles[i].getPos();
        float d = scaling(pos);
        if (d == 0) continue;
        // correct arch position
        float old_l = (pos - body_sphere).length();
        if (old_l - .00001 > body_radius ||
            old_l + .00001 < body_radius) {
            float new_l = (d*body_radius - d*old_l) + old_l;
            pos.makeUnitVector();
            pos *= new_l;
        }
        // correct the plane position
        pos.setZ( pos.z() - d * (pos.z() - zs[i]) );
        particles[i].setPos(pos);        
	}
}

void mediArchPlaneConstraintManipulator::draw() {
    // Draw constraining sphere for debug purpose
    glPushMatrix();
    glTranslatef(c[0],c[1],c[2]);
    //GLUT glutWireSphere(sqrt(sr),10,10);
    glPopMatrix();
    //cout << c << endl;
}
