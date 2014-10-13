#ifndef FACE_H
#define FACE_H
#include <vector>
#include "Vertex.h"

class Face
{
public:
	Face(int num, int idx[]);
	~Face();
	void drawFace(std::vector<Vertex> &vertexs);
	void show();

	int numVertex;
	int *vertex_induces;	
};

#endif;