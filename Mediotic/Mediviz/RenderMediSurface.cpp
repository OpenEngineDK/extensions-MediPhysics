#include "RenderMediSurface.h"
#include "../mediParticle.h"
#include <Logging/Logger.h>

RenderMediSurface::RenderMediSurface(mediSurface &m) : m(m) {
	this->generateVBOs();
}

void RenderMediSurface::updateVBOs() {
	for (unsigned int f=0;f<m.getFaces().size();f++) {
		for (int i=0;i<3;i++) {
			for (int j=0;j<3;j++) {
				vertices[f*3*3+i*3+ j]= m.getFaces()[f][i].getPos().e[j];
				normals[f*3*3+i*3+ j]= m.getFaces()[f][i].getNormal().e[j];
			}
		}
	}
}

void RenderMediSurface::generateVBOs() {
	vertices = (GLfloat*) malloc(sizeof(GLfloat)*m.getFaces().size()*3*3);
	normals = (GLfloat*) malloc(sizeof(GLfloat)*m.getFaces().size()*3*3);
	for (unsigned int f=0;f<m.getFaces().size();f++) {
		for (int i=0;i<3;i++) {
			for (int j=0;j<3;j++) {
				vertices[f*3*3+i*3+ j]= m.getFaces()[f][i].getPos().e[j];
				normals[f*3*3+i*3+ j]= m.getFaces()[f][i].getNormal().e[j];
			}
		}
	}
}

void RenderMediSurface::draw() {
    //blending and transparantcy
    glBlendFunc(GL_SRC_COLOR, GL_ONE);
	glBlendEquationEXT(GL_FUNC_REVERSE_SUBTRACT_EXT);
    //glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

	for(unsigned int fi=0; fi<m.getFaces().size(); fi++)
	{
        mediFace f = m.getFaces()[fi];
		glBegin(GL_TRIANGLES);
		for(int i=2;i>=0;i--) {
            glColor4f(0.4,0.4,0.4,0.9);
            // normals
			glNormal3fv(f[i].getNormal().e);
            // texture
            Vector2& t = objLoader->vt[ objLoader->f[fi].vt[i]-1 ]; 
            glTexCoord2f(t.x(), t.y());
            // vertex
			Vector3& v = f[i].getPos(); //automatic conversion to Vector3
			glVertex3f(v[0],v[1],v[2]);
		}
		glEnd();
	}
	glDepthMask(GL_TRUE);
}

void RenderMediSurface::drawWire() {
	for (unsigned int fi=0; fi<m.getFaces().size(); fi++) {
		mediFace f = m.getFaces()[fi];
		glBegin(GL_LINES);
		for (int i=2;i>=0;i--) {
			Vector3& v = f[i].getPos(); //automatic conversion to Vector3

			glNormal3fv(f[i].getNormal().e);
			glVertex3f(v[0],v[1],v[2]);
		}
		Vector3& v = f[0].getPos();
		glVertex3f(v[0],v[1],v[2]);
		glEnd();

		/*glPointSize(3);
		glBegin(GL_POINTS);
		for(int i=2;i>=0;i--)
		{
			Vector3& v = f[i].getPos(); //automatic conversion to Vector3

			glNormal3fv(f[i].getNormal().e);
			glColor3f(0,1,0);
			glVertex3f(v[0],v[1],v[2]);
		}
		glEnd();*/

	}

	glPointSize(4);
	glDisable(GL_LIGHTING);
	glBegin(GL_POINTS);

	vector<mediParticle>& particles = m.getParticles();
	
	for (vector<mediParticle>::iterator it = particles.begin();
        it!=particles.end(); it++) {
		Vector3& v = (*it).getPos(); //automatic conversion to Vector3
		glColor3f(1,0,0);
		glVertex3f(v[0],v[1],v[2]);
	}
	glEnd();
	glEnable(GL_LIGHTING);
}
