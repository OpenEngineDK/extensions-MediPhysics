#include "RenderMediSurface.h"
#include "../mediParticle.h"
#include <limits>
#include <Logging/Logger.h>

#include <iostream>
using namespace OpenEngine::Logging;

RenderMediSurface::RenderMediSurface(mediSurface &m) : m(m)
{
	this->generateVBOs();
}

void RenderMediSurface::updateVBOs()
{

	for(unsigned int f=0;f<m.getFaces().size();f++)
	{
		for(int i=0;i<3;i++)
		{
			for(int j=0;j<3;j++)
			{
				vertices[f*3*3+i*3+ j]= m.getFaces()[f][i].getPos().e[j];
				normals[f*3*3+i*3+ j]= m.getFaces()[f][i].getNormal().e[j];
			}
		}

	}

    /*	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboid[0]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, ( m.getFaces().size() * 3 * 3 )*sizeof(GLfloat), vertices, GL_STATIC_DRAW_ARB);

	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboid[1]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, ( m.getFaces().size() * 3 * 3 )*sizeof(GLfloat), normals, GL_STATIC_DRAW_ARB);
    */
}

void RenderMediSurface::generateVBOs()
{

	vertices = (GLfloat*) malloc(sizeof(GLfloat)*m.getFaces().size()*3*3);
	normals = (GLfloat*) malloc(sizeof(GLfloat)*m.getFaces().size()*3*3);

	for(unsigned int f=0;f<m.getFaces().size();f++)
	{
		for(int i=0;i<3;i++)
		{
			for(int j=0;j<3;j++)
			{
				vertices[f*3*3+i*3+ j]= m.getFaces()[f][i].getPos().e[j];
				normals[f*3*3+i*3+ j]= m.getFaces()[f][i].getNormal().e[j];
			}
		}

	}

    /*	glGenBuffersARB(3, vboid);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboid[0]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, ( m.getFaces().size() * 3 * 3 )*sizeof(GLfloat), vertices, GL_STATIC_DRAW_ARB);

	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboid[1]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, ( m.getFaces().size() * 3 * 3 )*sizeof(GLfloat), normals, GL_STATIC_DRAW_ARB);
    */
/*	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboid[2]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, ( objLoader->vt1.size() * 2 )*sizeof(GLfloat), &objLoader->vt1[0], GL_STATIC_DRAW_ARB);
*/

}

 #define BUFFER_OFFSET(i) ((char *)NULL + (i))


void RenderMediSurface::draw()
{
    //blending and transparantcy
    //glClearColor(0,0,0,0.5);
    //glEnable(GL_CULL_FACE);
    //glBlendFunc(GL_ONE, GL_SRC_ALPHA);

    //   glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_ONE);
    glBlendFunc(GL_SRC_COLOR, GL_ONE);
	glBlendEquationEXT(GL_FUNC_REVERSE_SUBTRACT_EXT);
//	glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
//	glBlendEquation(GL_FUNC_ADD_EXT);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);

    // light settings
    /*    GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
    GLfloat lm_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    */
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
	//glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

    //   	this->updateVBOs();

    /*	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboid[0]);
	glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboid[1]);	
	glNormalPointer(GL_FLOAT,0,BUFFER_OFFSET(0));
	//ColorPointer(4, UNSIGNED_BYTE, 0, BUFFER_OFFSET(256));

	// Enable arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	// Draw arrays
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei) m.getFaces().size()*3);
	//glDrawElements(GL_TRIANGLES, m.getFaces().size(), GL_UNSIGNED_INT, NULL);

	// Disable arrays
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	//DisableClientState(COLOR_ARRAY);
    */

}

void RenderMediSurface::drawWire()
{
	for(unsigned int fi=0; fi<m.getFaces().size(); fi++)
	{
		mediFace f = m.getFaces()[fi];
		glBegin(GL_LINES);
		for(int i=2;i>=0;i--)
		{
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
	
	for(vector<mediParticle>::iterator it = particles.begin(); it!=particles.end(); it++)
	{
		Vector3& v = (*it).getPos(); //automatic conversion to Vector3
		glColor3f(1,0,0);
		glVertex3f(v[0],v[1],v[2]);
	}
	glEnd();
	glEnable(GL_LIGHTING);

}

void RenderMediSurface::normalize(){
}
