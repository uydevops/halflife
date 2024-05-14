/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*
*	Description: This program is a basic 3D model viewer using OpenGL and GLUT.
*                 It allows the user to load a model file and view it on the screen.
*
*	Updates:
*	1-4-98	fixed initialization
*
****/

#include <stdio.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include "mathlib.h"
#include "../../public/steam/steamtypes.h" // Defines int32, required by studio.h
#include "../../engine/studio.h"
#include "mdlviewer.h"

#pragma warning(disable : 4244) // Conversion from 'double ' to 'float ', possible loss of data
#pragma warning(disable : 4305) // Truncation from 'const double ' to 'float '

// Variables for OpenGL rendering
vec3_t g_vright; // Needs to be set to viewer's right in order for chrome to work
float g_lambert = 1.5;
float gldepthmin = 0;
float gldepthmax = 10.0;

// Function to clear OpenGL buffers
void R_Clear(void)
{
    glDepthFunc(GL_LEQUAL);
    glDepthRange(gldepthmin, gldepthmax);
    glDepthMask(1);
}

// Temporary model object for rendering
static StudioModel tempmodel;

// Function to render the model
void mdlviewer_display()
{
    R_Clear();

    tempmodel.SetBlending(0, 0.0);
    tempmodel.SetBlending(1, 0.0);

    static float prev;
    float curr = GetTickCount() / 1000.0;
    tempmodel.AdvanceFrame(curr - prev);
    prev = curr;

    tempmodel.DrawModel();
}

// Function to initialize the model viewer with a specified model
void mdlviewer_init(char *modelname)
{
    tempmodel.Init(modelname);
    tempmodel.SetSequence(0);

    tempmodel.SetController(0, 0.0);
    tempmodel.SetController(1, 0.0);
    tempmodel.SetController(2, 0.0);
    tempmodel.SetController(3, 0.0);
    tempmodel.SetMouth(0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50., 1., .1, 10.);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0, 0, 0.5, 0);
}
// Function to switch to the next sequence of the model
void mdlviewer_nextsequence(void)
{
    int iSeq = tempmodel.GetSequence();
    if (iSeq == tempmodel.SetSequence(iSeq + 1))
    {
        tempmodel.SetSequence(0);
    }
}

// Function to handle panning of the model
void pan(int x, int y)
{
    transx += (x - ox) / 500.;
    transy -= (y - oy) / 500.;
    ox = x;
    oy = y;
    glutPostRedisplay();
}

// Function to handle zooming of the model
void zoom(int x, int y)
{
    transz += (x - ox) / 20.;
    ox = x;
    glutPostRedisplay();
}

// Function to handle rotation of the model
void rotate(int x, int y)
{
    rotx += x - ox;
    if (rotx > 360.)
        rotx -= 360.;
    else if (rotx < -360.)
        rotx += 360.;
    roty += y - oy;
    if (roty > 360.)
        roty -= 360.;
    else if (roty < -360.)
        roty += 360.;
    ox = x;
    oy = y;
    glutPostRedisplay();
}

// Function to handle mouse motion
void motion(int x, int y)
{
    if (mot == PAN)
        pan(x, y);
    else if (mot == ROT)
        rotate(x, y);
    else if (mot == ZOOM)
        zoom(x, y);
}

// Function to handle mouse clicks
void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        switch (button)
        {
        case GLUT_LEFT_BUTTON:
            mot = PAN;
            motion(ox = x, oy = y);
            break;
        case GLUT_RIGHT_BUTTON:
            mot = ROT;
            motion(ox = x, oy = y);
            break;
        case GLUT_MIDDLE_BUTTON:
            break;
        }
    }
    else if (state == GLUT_UP)
    {
        mot = 0;
    }
}

// Function to display help information
void help(void)
{
    printf("left mouse     - pan\n");
    printf("right mouse    - rotate\n");
}

// Function to initialize the application
void init(char *arg)
{
    mdlviewer_init(arg);

    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50., 1., .1, 10.);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0, 0, 0.5, 0);
}

