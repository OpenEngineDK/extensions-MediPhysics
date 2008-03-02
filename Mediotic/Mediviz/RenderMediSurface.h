#pragma once

#include "../mediSurface.h"
#include "../OpenGL.h"
#include "../OBJLoader/ObjLoader.h"
//#include <GL/glew.h>

class RenderMediSurface
{
private: 
	mediSurface &m;
    ObjLoader* objLoader; //the actual model needed to access the texture coordinates.

	GLuint vboid[3];

	GLfloat *vertices;
	GLfloat *normals;

	void normalize();
	void generateVBOs();
	void updateVBOs();

public:
//	RenderMediSurface(){};
	RenderMediSurface(mediSurface &m);
	void draw();
	void drawWire();

    void SetObjLoader(ObjLoader* loader) {
        this->objLoader = loader;
    }
    // Texture hack
    int textureID;
};
