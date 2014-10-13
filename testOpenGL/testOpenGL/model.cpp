#include "model.h"
#include <string>
#include <sstream>
#include <GL/glut.h>
using namespace std;

Model::Model()
{
}

Model::Model(std::string fileName, int numVertex, int numFace, float xAxis_init, float yAxis_init, float zAxis_init)
{
	xRotate_init = xAxis_init;
	yRotate_init = yAxis_init;
	zRotate_init = zAxis_init;
	readPLY(fileName, numVertex, numFace);
	findBoundBox();
}

Model::~Model()
{
	for(int i=0;i<8;i++)
		delete boundBox[i];
}

bool Model::readPLY(string fileName, int numVertex, int numFace)
{
	FILE *file = fopen(fileName.c_str(), "rb");
	if(file == NULL){
		cout<<"Unable to open "<<fileName<<endl;
		return false;
	}

	/* 計算header結束的位置 */
	stringstream ss;
	ss<<numVertex<<numFace;
	int digit = ss.str().length();
	int fileptr = 317+digit;

	/* load vertex */
	float vertex[3];
	unsigned char color[3];
	fseek(file, fileptr*sizeof(char), SEEK_SET);
	for(int i=0;i<numVertex;i++){
		fread(vertex, sizeof(float), 3, file);
		fread(color, sizeof(unsigned char), 3, file);
		Vertex v(vertex, color);
		vertexs.push_back(v);
	}

	/* load face */
	int numOfVertex;
	for(int i=0;i<numFace;i++){
		int vertex_indices[10];
		unsigned char temp;
		fread(&temp, sizeof(unsigned char), 1, file);
		numOfVertex = (int)temp;
		fread(vertex_indices, sizeof(int), numOfVertex, file);
		/*cout<<numOfVertex<<" ";
		for(int j=0;j<numOfVertex;j++)
			cout<<vertex_indices[j]<<" ";
		cout<<endl;*/
		Face f(numOfVertex, vertex_indices);
		faces.push_back(f);
	}
			
	fclose(file);

	normalizeVertex();
	return true;
}

void Model::normalizeVertex()
{
	float xMean = 0.0f, yMean = 0.0f, zMean = 0.0f;
	for(unsigned int i=0;i<vertexs.size();i++){
		xMean += vertexs[i].position[0];
		yMean += vertexs[i].position[1];
		zMean += vertexs[i].position[2];
	}
	xMean /= vertexs.size();
	yMean /= vertexs.size();
	zMean /= vertexs.size();
	for(unsigned int i=0;i<vertexs.size();i++){
		vertexs[i].position[0] -= xMean;
		vertexs[i].position[1] -= yMean;
		vertexs[i].position[2] -= zMean;
	}
}

void Model::drawModel()
{
	//glPushMatrix();
	//glLoadIdentity();
	glRotatef(zRotate_init+zRotate, 0.0f, 0.0f, 1.0f);
	glRotatef(xRotate_init+xRotate, 1.0f, 0.0f, 0.0f);
	glRotatef(yRotate_init+yRotate, 0.0f, 1.0f, 0.0f);

	glScalef(15.0f, 15.0f, 15.0f);
	for(unsigned int i=0;i<faces.size();i++){
		faces[i].drawFace(vertexs);
	}
	//glPopMatrix();
}

void Model::drawBoundBox()
{
	glColor3ub(50, 250, 100);
	glBegin(GL_LINE_LOOP);
		glVertex3fv(boundBox[0]->position);
		glVertex3fv(boundBox[1]->position);
		glVertex3fv(boundBox[2]->position);
		glVertex3fv(boundBox[3]->position);
	glEnd();
	glBegin(GL_LINE_LOOP);
		glVertex3fv(boundBox[4]->position);
		glVertex3fv(boundBox[5]->position);
		glVertex3fv(boundBox[6]->position);
		glVertex3fv(boundBox[7]->position);
	glEnd();
	glBegin(GL_LINES);
		glVertex3fv(boundBox[0]->position);
		glVertex3fv(boundBox[4]->position);
		glVertex3fv(boundBox[1]->position);
		glVertex3fv(boundBox[5]->position);
		glVertex3fv(boundBox[2]->position);
		glVertex3fv(boundBox[6]->position);
		glVertex3fv(boundBox[3]->position);
		glVertex3fv(boundBox[7]->position);
	glEnd();
}

Vertex Model::getBoundBox(unsigned int i)
{
	if(i<8)
		return *boundBox[i];
	else{
		std::cerr<<"unable to acess boundBox["<<i<<"]."<<std::endl;
		exit(0);
	}
}

void Model::showVertexs()
{
	for(unsigned int i=0;i<vertexs.size();i++)
		vertexs.at(i).show();
}

void Model::showFaces()
{
	for(unsigned int i=0;i<faces.size();i++)
		faces.at(i).show();
}

void Model::rotateModel(float x, float y, float z)
{
	xRotate += x;
	yRotate += y;
	zRotate += z;

	if(xRotate>360.0f) 
		xRotate -= 360.0f;
	else if(xRotate<-360.0f)
		xRotate += 360.0f;
	
	if(yRotate>360.0f) 
		yRotate -= 360.0f;
	else if(yRotate<-360.0f)
		yRotate += 360.0f;
	
	if(zRotate>360.0f) 
		zRotate -= 360.0f;
	else if(zRotate<-360.0f)
		zRotate += 360.0f;
}

void Model::resetRotation(){
	xRotate = 0;
	yRotate = 0;
	zRotate = 0;
}

void Model::showRotate()
{
	cout<<"["<<xRotate<<" "<<yRotate<<" "<<zRotate<<"]"<<endl;
}

void Model::findBoundBox()
{
	float Max[3] = {0, 0, 0}, Min[3] = {1000000, 1000000, 1000000};
	for(unsigned int i=0;i<vertexs.size();i++){
		for(int j=0;j<3;j++){
			if(Max[j] < vertexs[i].position[j]){
				Max[j] = vertexs[i].position[j];
			}
			if(Min[j] > vertexs[i].position[j]){
				Min[j] = vertexs[i].position[j];
			}
		}
	}
	width[0] = Max[0] - Min[0];
	width[1] = Max[1] - Min[1];
	width[2] = Max[2] - Min[2];
	float pos[3];

	pos[0] = Max[0]; pos[1] = Min[1]; pos[2] = Min[2];      //(x,0,0)
	boundBox[0] = new Vertex(pos, NULL);
	pos[0] = Max[0]; pos[1] = Max[1]; pos[2] = Min[2];      //(x,y,0)
	boundBox[1] = new Vertex(pos, NULL);
	pos[0] = Max[0]; pos[1] = Max[1]; pos[2] = Max[2];		//(x,y,z)
	boundBox[2] = new Vertex(pos, NULL);
	pos[0] = Max[0]; pos[1] = Min[1]; pos[2] = Max[2];		//(x,0,z)
	boundBox[3] = new Vertex(pos, NULL);
	pos[0] = Min[0]; pos[1] = Min[1]; pos[2] = Min[2];		//(0,0,0)
	boundBox[4] = new Vertex(pos, NULL);
	pos[0] = Min[0]; pos[1] = Max[1]; pos[2] = Min[2];		//(0,y,0)
	boundBox[5] = new Vertex(pos, NULL);
	pos[0] = Min[0]; pos[1] = Max[1]; pos[2] = Max[2];		//(0,y,z)
	boundBox[6] = new Vertex(pos, NULL);
	pos[0] = Min[0]; pos[1] = Min[1]; pos[2] = Max[2];		//(0,0,z)
	boundBox[7] = new Vertex(pos, NULL);
}