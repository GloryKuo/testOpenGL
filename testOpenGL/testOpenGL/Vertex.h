#ifndef VERTEX_H
#define VERTEX_H

class Vertex
{
public:
	Vertex(float pos[], unsigned char clr[]);
	void show();

	float position[3];
	unsigned char color[3];
};
#endif
