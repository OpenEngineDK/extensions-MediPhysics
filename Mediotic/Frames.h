/*****************************************

frames.h

 General purpose frames per second counter for OpenGL/GLUT GNU/Linux
 programs. Displays "Frames per second: N" at an arbitrary position in
 the window. Saves and restores the app's modelview and projection matrices,
 colour, and lighting.

 Author: Toby Howard. toby@cs.man.ac.uk.
 Version 2.0, 23 March 1999

 ====================

 Usage: to add an on-screen frames per second counter to your program, save
 this file alongside your app as "frames.h", and add:

    #include "frames.h"

 immediately after all your app's other #includes; then bracket all the
 code in your display() function, before swapping buffers, with

   frameStart();

 and

   frameEnd(void *font, GLclampf r, GLclampf g, GLclampf b, 
            float x, float y);

     font:    font to use, e.g., GLUT_BITMAP_HELVETICA_10
     r, g, b: RGB text colour
     x, y:    text position in window: range [0,0] (bottom left of window)
              to [1,1] (top right of window).

 ====================

 Example:

    void display(void) {
      glClear(GL_COLOR_BUFFER_BIT);
 
      frameStart();
     
      // all the graphics code
 
      frameEnd(GLUT_BITMAP_HELVETICA_10, 1.0, 1.0, 1.0, 0.05, 0.95);

      glutSwapBuffers();
    }
*****************************************/

#include "timeexp.h" // Modified by JM
#include <stdio.h>

int counter=0;
int noFrames = 30;
float FPS;

void frameStart(void) {
  if (counter!=noFrames) 
  {
	counter++;
  }
  else
  {
  counter = 0;
  FPS = 1000.0*noFrames/ElapsedTime();
  StartTimer();
  }
}

void frameEnd(void *font, GLclampf r, GLclampf g, GLclampf b, 
              GLfloat x, GLfloat y) {
  /* font: font to use, e.g., GLUT_BITMAP_HELVETICA_10
     r, g, b: text colour
     x, y: text position in window: range [0,0] (bottom left of window)
           to [1,1] (top right of window). */

  //float elapsedTime; 
  char str[30]; 
  char *ch; 
  GLint matrixMode;
  GLboolean lightingOn;

  
   sprintf(str, "Frames per second: %.2f", FPS );
  //sprintf(str, "Elapsed time: %.2f", ElapsedTime());

 lightingOn= glIsEnabled(GL_LIGHTING);        /* lighting on? */
 if (lightingOn) glDisable(GL_LIGHTING);

 glGetIntegerv(GL_MATRIX_MODE, &matrixMode);  /* matrix mode? */

 glMatrixMode(GL_PROJECTION);
 glPushMatrix();
   glLoadIdentity();
   gluOrtho2D(0.0, 1.0, 0.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
     glLoadIdentity();
     glPushAttrib(GL_COLOR_BUFFER_BIT);       /* save current colour */
       glColor3f(r, g, b);
       glRasterPos3f(x, y, 0.0);
       for(ch= str; *ch; ch++) {
	 //GLUT glutBitmapCharacter(font, (int)*ch);
       }
     glPopAttrib();
   glPopMatrix();
   glMatrixMode(GL_PROJECTION);
 glPopMatrix();
 glMatrixMode(matrixMode);
 if (lightingOn) glEnable(GL_LIGHTING);
}

/* end of frames.h */
