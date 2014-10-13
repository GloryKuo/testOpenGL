#include "Face.h"
#include <iostream>
#include <GL/glut.h>

Face::Face(int num, int idx[])
{
	numVertex = num;
	vertex_induces = new int[num];
	memcpy(vertex_induces, idx, num*sizeof(int));
}

Face::~Face()
{
	//delete [] vertex_induces;
}

void Face::drawFace(std::vector<Vertex> &vertexs)
{
	glBegin(GL_TRIANGLES);
		for(int i=0;i<numVertex;i++){
			glColor3ubv( vertexs[vertex_induces[i]].color );
			glVertex3fv( vertexs[vertex_induces[i]].position );
		}
	glEnd();
	//glFlush();
}

void Face::show()
{
	std::cout<<numVertex<<" ";
	for(int i=0;i<numVertex;i++)
		std::cout<<vertex_induces[i]<<" ";
	std::cout<<std::endl;
}