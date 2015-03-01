#include "TracBall.h"
#include <GL\glut.h>

TracBall::TracBall()
{
	angle = 0.0; 
	trackingMouse = false; 
	trackballMove = false;
	lastPos[0] = 0.0f; lastPos[1] = 0.0f; lastPos[2] = 0.0f; 
	memset(RotMat, 0, 16*sizeof(double));
	RotMat[0] = 1.0; RotMat[5] = 1.0; RotMat[10] = 1.0; RotMat[15] = 1.0;
}

TracBall::~TracBall()
{
}

void TracBall::trackball_ptov(int x, int y, int width, int height, float v[3])
{
	float d, a;

	/* project x,y onto a hemi-sphere centered within width, height */
	v[0] = (2.0F*x - width) / width;
	v[1] = (height - 2.0F*y) / height;
	d = (float) sqrt(v[0]*v[0] + v[1]*v[1]);
	v[2] = (float) cos((M_PI/2.0F) * ((d < 1.0F) ? d : 1.0F));
	a = 1.0F / (float) sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	v[0] *= a;
	v[1] *= a;
	v[2] *= a;
}

void TracBall::Start(int x, int y, int width, int height)
{
	trackingMouse = true;
	startX = x; startY = y;
	curx = x; cury = y;
	trackball_ptov(x, y, width, height, lastPos);
	trackballMove=true;
}

void TracBall::Stop(int x, int y)
{
	trackingMouse = false;
	angle = 0.0F;
	trackballMove = false;
}

void TracBall::Motion(int x, int y, int width, int height)
{
	float curPos[3], dx, dy, dz;

	trackball_ptov(x, y, width, height, curPos);
	if(trackingMouse)
	{
		dx = curPos[0] - lastPos[0];
		dy = curPos[1] - lastPos[1];
		dz = curPos[2] - lastPos[2];

		if (dx || dy || dz) {
			angle = 90.0F * sqrt(dx*dx + dy*dy + dz*dz);

			axis[0] = lastPos[1]*curPos[2] - lastPos[2]*curPos[1];
			axis[1] = lastPos[2]*curPos[0] - lastPos[0]*curPos[2];
			axis[2] = lastPos[0]*curPos[1] - lastPos[1]*curPos[0];

			lastPos[0] = curPos[0];
			lastPos[1] = curPos[1];
			lastPos[2] = curPos[2];

			Update();
		}
	} 
}

void TracBall::Update(void)
{
	/// compute rotation matrix
	glPushMatrix();
	glLoadIdentity();
	glRotated(angle, axis[0], axis[1], axis[2]);			
	glMultMatrixd((GLdouble*)RotMat);
	glGetDoublev(GL_MODELVIEW_MATRIX, (GLdouble*)RotMat);	
	glPopMatrix();	
}

void TracBall::Apply(void)
{
	//glLoadIdentity();		
	glMultMatrixd((GLdouble*)RotMat);
}