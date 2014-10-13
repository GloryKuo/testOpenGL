// ----------------------------------------------------------------------------
// Simple sample to prove that Assimp is easy to use with OpenGL.
// It takes a file name as command line parameter, loads it using standard
// settings and displays it.
//
// If you intend to _use_ this code sample in your app, do yourself a favour 
// and replace immediate mode calls with VBOs ...
//
// The vc8 solution links against assimp-release-dll_win32 - be sure to
// have this configuration built.
// ----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>

#include <GL/glut.h>

#include "Model_assimp.h"


Model_assimp *scene = NULL;

// current rotation angle
static float angle = 0.f;


// ----------------------------------------------------------------------------
void reshape(int width, int height)
{
	const double aspectRatio = (float) width / height, fieldOfView = 45.0;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fieldOfView, aspectRatio,
		1.0, 1000.0);  /* Znear and Zfar */
	glViewport(0, 0, width, height);
}
//// ----------------------------------------------------------------------------
void do_motion (void)
{
	static GLint prev_time = 0;
	static GLint prev_fps_time = 0;
	static int frames = 0;

	int time = glutGet(GLUT_ELAPSED_TIME);
	angle += (time-prev_time)*0.01;
	prev_time = time;

	frames += 1;
	if ((time - prev_fps_time) > 1000) // update every seconds
    {
        int current_fps = frames * 1000 / (time - prev_fps_time);
        printf("%d fps\n", current_fps);
        frames = 0;
        prev_fps_time = time;
    }


	glutPostRedisplay ();
}

// ----------------------------------------------------------------------------
void display(void)
{
	float tmp;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.f,0.f,3.f,0.f,0.f,-5.f,0.f,1.f,0.f);

	// rotate it around the y axis
	glRotatef(angle,0.f,1.f,0.f);

	scene->drawModel();

	glutSwapBuffers();

	do_motion();
}

// ----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	/*struct aiLogStream stream;*/
	scene = new Model_assimp();

	glutInitWindowSize(900,600);
	glutInitWindowPosition(100,100);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);

	glutCreateWindow("Assimp - Very simple OpenGL sample");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	if(! scene->loadAsset(argc >= 2 ? argv[1] : "../models/Bulbasaur/Bulbasaur.obj")){
		if( argc != 1 || (!scene->loadAsset("../models/Bulbasaur/Bulbasaur.obj")) &&
			(!scene->loadAsset("../models/Bulbasaur/BR_Bulbasaur.obj"))){
			return -1;
		}
	}

	glClearColor(0.1f,0.1f,0.1f,1.f);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    // Uses default lighting parameters

	glEnable(GL_DEPTH_TEST);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);

	// XXX docs say all polygons are emitted CCW, but tests show that some aren't.
	if(getenv("MODEL_IS_BROKEN"))  
		glFrontFace(GL_CW);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

	glutGet(GLUT_ELAPSED_TIME);
	glutMainLoop();

	delete scene;
	return 0;
}
