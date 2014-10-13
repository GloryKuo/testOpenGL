#ifndef MODEL_H
#define MODEL_H
#include <iostream>
#include <vector>
#include "Vertex.h"
#include "Face.h"

class Model
{
public:
	Model();   //為繼承使用
	Model(std::string fileName, int numVertex, int numFace, float xAxis_init=0.0f, float yAxis_init=0.0f, float zAxis_init=0.0f);
	~Model();
	bool readPLY(std::string fileName, int numVertex, int numFace);
	void normalizeVertex();
	virtual void drawModel();
	void drawBoundBox();
	void rotateModel(float x, float y, float z);
	void resetRotation();
	Vertex getBoundBox(unsigned int i);

	void showRotate();
	void showVertexs();
	void showFaces();
protected:
	virtual void findBoundBox();

	float xRotate_init, yRotate_init, zRotate_init;
	float xRotate, yRotate, zRotate;
	float width[3];          //boundBox的寬度(x,y,z)
	Vertex *boundBox[8];
	std::vector<Vertex> vertexs;
	std::vector<Face> faces;
};
#endif