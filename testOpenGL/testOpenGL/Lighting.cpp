#include "TracBall.h"
#include <GL\glut.h>
#include <iostream>
#include <vector>
#include <algorithm>

/*截圖用*/
#include <Windows.h>
#include <time.h>
using namespace std;

GLfloat tri_v[3][2] = {{-1.0f, -0.58f}, {1.0f, -0.58f}, {0.0f, 1.15f}};

int winWidth, winHeight;

int timer_flag = 1;

int timer_cnt = 0;

int timer_speed = 10;

int model = 1, mode = 2;

// translate variable
float translate_x = 0.0f, translate_y = 0.0f, translate_z = 0.0f;

//Lighting parameters

GLfloat ambientLight[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat diffuseLight[] = { 0.85f, 0.85f, 0.85f, 1.0f };
GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat pos[]={-40.0f, -40.0f, 25.0f, 1.0f};
float sideLength = 1000.0f;
float markSideLength = 20.0f;

TracBall myTrackBall;

/*截圖用*/
BOOL WriteBitmapFile(char * filename,int width,int height,unsigned char * bitmapData); 
void SaveScreenShot();
//////////////////
void InitGL(void);
void drawPlane(double w, double h, int sides, double nx, double ny, double nz);


void My_Reshape(int w, int h)
{
	GLfloat aspectRatio;

	winWidth = w;
	winHeight = h;


	if(h == 0) h = 1;

	glViewport(0, 0, w, h);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 200.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0,0.0,50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//InitGL();
}

void drawTriangle()
{
	 glBegin(GL_TRIANGLES);
        glColor3f(0.0, 1.0, 0.0);
        for(int i = 0; i < 3 ; i++){
			glVertex3f(tri_v[i][0]*2, tri_v[i][1]*2, 0);
		}
	glEnd();
}




void My_Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	

	glLoadIdentity();
	
	/// Setup the viewing camera
	gluLookAt(0.0, 0.0,50.0, 
		      0.0, 0.0, 0.0, 
			  0.0, 1.0, 0.0);

	//Translation
	glTranslatef(translate_x, translate_y, translate_z);

	//TracBall
	myTrackBall.Apply();
	

	//Rendering 
	if(mode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	switch(model){

		case 1:
			//glColor3ub(180,180,180);
			//glutSolidTeapot(10.0);			
			//glutSolidSphere(10.0, 128, 128);
			//glBegin(GL_QUADS);
			//	glNormal3f(0.0f, 0.0f, 1.0f);
			//	glVertex3f(-sideLength, -sideLength, 0.0f);
			//	glNormal3f(0.0f, 0.0f, 1.0f);
			//	glVertex3f(sideLength, -sideLength, 0.0f);
			//	glNormal3f(0.0f, 0.0f, 1.0f);
			//	glVertex3f(sideLength, sideLength, 0.0f);
			//	glNormal3f(0.0f, 0.0f, 1.0f);
			//	glVertex3f(-sideLength, sideLength, 0.0f);
			//	//glNormal3f(0.0f, 0.0f, 1.0f);
			//glEnd();
			glPushMatrix();			
			glColor3f(1.0f, 1.0f, 1.0f);
			drawPlane(sideLength, sideLength, 200, 0.0, 0.0, 1.0);
			glPopMatrix();

			/*glPushMatrix();
			glTranslatef(markSideLength, markSideLength, 0.0f);
			glRotatef(90, 1.0f, 0.0f, 0.0f);
			drawPlane(sideLength, sideLength, 100, 0.0, 1.0, 0.0);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(markSideLength, markSideLength, 0.0f);
			glRotatef(-90, 0.0f, 1.0f, 0.0f);
			drawPlane(sideLength, sideLength, 100, 1.0, 0.0, 0.0);
			glPopMatrix();*/

			glPushMatrix();	
			glBegin(GL_LINE_LOOP);
				glColor3f(0.0f, 0.0f, 0.0f);
				glLineWidth(3.0f);
				glVertex3f(-markSideLength, -markSideLength, 0.0f);
				glVertex3f(markSideLength, -markSideLength, 0.0f);
				glVertex3f(markSideLength, markSideLength, 0.0f);
				glVertex3f(-markSideLength, markSideLength, 0.0f);
			glEnd();
			glPopMatrix();

			break;
		case 2:
			glColor3ub(0,255,0);
			glutSolidTorus(5.0, 10.0, 30, 30);		
			break;
		case 3:
		
			glColor3ub(0,0,255);

			// Create a quadric object 
			GLUquadricObj *p; 
			// Set up object
			p = gluNewQuadric(); 
			// Set up render style
			gluQuadricDrawStyle(p, GLU_FILL);
			// Draw cylindric object
			gluCylinder(p, 10.0, 5.0, 
					10.0, 30, 30);
			break;

		case 4:
			// draw a triangle
			drawTriangle();
			break;
	}	

	
	/// Flush drawing commands
	glutSwapBuffers();

}

void InitGL(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	


	// Setup and enable light 0
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);	
	/*glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.02);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);*/
	//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);    //影響specular

	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_LIGHT0);

}

void SetMaterial() 
{ 
   float material_ambient[]  = { 0.1, 0.1, 0.1, 1.0}; 
   float material_diffuse[]  = { 1.0, 1.0, 1.0, 1.0}; 
   float material_specular[] = { 0.5, 0.5, 0.5, 1.0}; 
   GLfloat mat_shininess[] = { 100.0 };

   glMaterialfv( GL_FRONT, GL_AMBIENT,  material_ambient); 
   glMaterialfv( GL_FRONT, GL_DIFFUSE,  material_diffuse); 
   /*glMaterialfv( GL_FRONT, GL_SPECULAR, material_specular); 
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);*/

} 


void My_Timer(int value)
{
	if(value == 0) return;

	timer_cnt++;
	timer_cnt = timer_cnt % 256;

	glutPostRedisplay();
	glutTimerFunc(timer_speed, My_Timer, timer_flag);
}

void My_Keyboard(unsigned char key, int x, int y)
{
	switch( key ){
		case 'z': case 'Z':
			system("cls");
			cout << "z button down" << endl;
			translate_z += 0.5f;
			printf("eye z = %f", translate_z);
			
			break;
		case 'x': case 'X':
			system("cls");
			cout << "x button down" << endl;
			translate_z -= 0.5f;
			printf("eye z = %f", translate_z);
			break;
		case 'q': case 'Q':
			exit(0);
			break;
		case 'a' : case 'A':
			cout << "a button down" << endl;
			//diffuseLight[0] += 0.1f;
			//diffuseLight[1] += 0.1f;
			//diffuseLight[2] += 0.1f;
			//cout << diffuseLight[2] << endl;
			InitGL();
			//glutPostRedisplay();
			break;
		case 'p' : case 'P':
			SaveScreenShot();
			break;
	}
}

void My_Mouse(int button, int state, int x, int y)
{
	switch (button){
		case GLUT_LEFT_BUTTON:	
			switch(state){
				case GLUT_DOWN:
					myTrackBall.Start(x, y, winWidth, winHeight);
					break;
				case GLUT_UP:
					myTrackBall.Stop(x, y);
					break;
			} 
			break;
		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_DOWN)
				cout << "Mouse middle button down" << endl;
			break;
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN)
				cout << "Mouse right button down" << endl;
			break;
		default:
			break;
	}
}

void My_SpecialKeys( int key, int x, int y )
{
	switch( key ) {
		case GLUT_KEY_F1 :
			cout << "This is F1 key" << endl;
			break;
		case GLUT_KEY_PAGE_UP :
			cout << "This is PageUp key" << endl;
			break;	
		case GLUT_KEY_LEFT :
			cout << "This is Left key" << endl;
			translate_x -= 0.5f;
			break;	
		case GLUT_KEY_RIGHT :
			cout << "This is Right key" << endl;
			translate_x += 0.5f;
			break;
		case GLUT_KEY_UP :
			cout << "This is Up key" << endl;
			translate_y += 0.5f;
			break;	
		case GLUT_KEY_DOWN :
			cout << "This is Down key" << endl;
			translate_y -= 0.5f;
			break;

	}
}

void My_MouseMotion(int x, int y)
{
	myTrackBall.Motion(x,y,winWidth,winHeight);
	InitGL();
	glutPostRedisplay();
}

void menu_main_func(int value)
{	
	model = value;
	glutPostRedisplay();
}

void menu_lvl1_func(int value)
{	
	mode = value;
	glutPostRedisplay();
}

void My_Menu()
{
	int menu_main, menu_lvl1, menu_lvl2;

	menu_main = glutCreateMenu(menu_main_func);
	menu_lvl1 = glutCreateMenu(menu_lvl1_func);	

	glutSetMenu( menu_main );	
	glutAddSubMenu("Modes", menu_lvl1);
	glutAddMenuEntry("Teapot", 1);
	glutAddMenuEntry("Torus", 2);			
	glutAddMenuEntry("Cylinder", 3);

	glutSetMenu( menu_lvl1 );	
	glutAddMenuEntry("Wireframe",1);	
	glutAddMenuEntry("Solid",2);	

	/// Bind menu to right mouse button
	glutSetMenu( menu_main );
	glutAttachMenu(GLUT_RIGHT_BUTTON); 

}

int main(int argc, char* argv[])
{
	// create a window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Lighting Test");

	//Register callback functions
	glutReshapeFunc	( My_Reshape );
	glutDisplayFunc	( My_Display );
	glutKeyboardFunc( My_Keyboard );
	glutMouseFunc	( My_Mouse );	
	glutSpecialFunc	( My_SpecialKeys );
	glutMotionFunc	( My_MouseMotion );
	glutTimerFunc	(timer_speed, My_Timer, timer_flag);

	/// Create GLUT menu system
	My_Menu();

	/// Initialize OpenGL
	InitGL();		

	SetMaterial();
	/// Entering main loop
	glutMainLoop();
	

}

//WriteBitmapFile
//根據bitmapData的（RGB）數據，保存bitmap
//filename是要保存到物理硬盤的文件名（包括路徑）
BOOL WriteBitmapFile(char * filename,int width,int height,unsigned char * bitmapData)
{
	//填充BITMAPFILEHEADER
	BITMAPFILEHEADER bitmapFileHeader;
	memset(&bitmapFileHeader,0,sizeof(BITMAPFILEHEADER));
	bitmapFileHeader.bfSize = sizeof(BITMAPFILEHEADER);
	bitmapFileHeader.bfType = 0x4d42;	//BM
	bitmapFileHeader.bfOffBits =sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//填充BITMAPINFOHEADER
	BITMAPINFOHEADER bitmapInfoHeader;
	memset(&bitmapInfoHeader,0,sizeof(BITMAPINFOHEADER));
	bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader.biWidth = width;
	bitmapInfoHeader.biHeight = height;
	bitmapInfoHeader.biPlanes = 1;
	bitmapInfoHeader.biBitCount = 24;
	bitmapInfoHeader.biCompression = BI_RGB;
	bitmapInfoHeader.biSizeImage = width * abs(height) * 3;

	//////////////////////////////////////////////////////////////////////////
	FILE * filePtr;			
	unsigned char tempRGB;	
	int imageIdx;

	for (imageIdx = 0;imageIdx < bitmapInfoHeader.biSizeImage;imageIdx +=3)
	{
		tempRGB = bitmapData[imageIdx];
		bitmapData[imageIdx] = bitmapData[imageIdx + 2];
		bitmapData[imageIdx + 2] = tempRGB;
	}

	filePtr = fopen(filename,"wb");
	if (NULL == filePtr)
	{
		return FALSE;
	}

	fwrite(&bitmapFileHeader,sizeof(BITMAPFILEHEADER),1,filePtr);

	fwrite(&bitmapInfoHeader,sizeof(BITMAPINFOHEADER),1,filePtr);

	fwrite(bitmapData,bitmapInfoHeader.biSizeImage,1,filePtr);

	fclose(filePtr);
	return TRUE;
}

void SaveScreenShot()
{
	int clnWidth,clnHeight;	//client width and height
	static void * screenData;
	RECT rc;
	int len = 600 * 600 * 3;
	screenData = malloc(len);
	memset(screenData,0,len);
	glReadPixels(0, 0, 600, 600, GL_RGB, GL_UNSIGNED_BYTE, screenData);

	time_t t = time(0);
	struct tm *now = localtime(&t);
	char lpstrFilename[256] = {0};
	sprintf_s(lpstrFilename,sizeof(lpstrFilename),"%d.%d.%d.bmp",now->tm_hour, now->tm_min, now->tm_sec);
	cout<<"save the screen shot at\n"<<lpstrFilename<<endl;

	WriteBitmapFile(lpstrFilename,600,600,(unsigned char*)screenData);
	
	free(screenData);

}

/////////////////////////////////////////////
void drawPlane(double w, double h, int sides, double nx, double ny, double nz){
  glPushMatrix();
  for(int i=0; i<sides; i++){
    for(int j=0; j<sides; j++){
      double ax = -w/2.0 + w/double(sides) * double(i);
      double bx = -h/2.0 + h/double(sides) * double(j);
      glNormal3d( nx, ny, nz);
      glBegin(GL_QUADS);
      glVertex3d( ax,                 bx,                 0.0);
      glVertex3d( ax+w/double(sides), bx,                 0.0);
      glVertex3d( ax+w/double(sides), bx+h/double(sides), 0.0);
      glVertex3d( ax,                 bx+h/double(sides), 0.0);
      glEnd();
    }
  }
  glPopMatrix();
}


