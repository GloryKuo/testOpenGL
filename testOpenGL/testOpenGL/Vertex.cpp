#include "Vertex.h"
#include <iostream>

Vertex::Vertex(float pos[], unsigned char clr[] = NULL)
{
	memcpy(position, pos, 3*sizeof(float));
	if(clr !=  NULL)
		memcpy(color, clr, 3*sizeof(unsigned char));
}

void Vertex::show()
{
	std::cout<<"("<<position[0]<<", "<<position[1]<<", "<<position[2]<<") ";
	std::cout<<"["<<(int)color[0]<<", "<<(int)color[1]<<", "<<(int)color[2]<<"]"<<std::endl;
}