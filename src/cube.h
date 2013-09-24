#ifndef CUBE_H
#define CUBE_H

int cubeVertexCount = 24;

float cubeVertices[] = {
	-0.5,-0.5,-0.5,
	-0.5, 0.5,-0.5,
	 0.5, 0.5,-0.5,
	 0.5,-0.5,-0.5,

	-0.5,-0.5, 0.5,
	-0.5, 0.5, 0.5,
	 0.5, 0.5, 0.5,
	 0.5,-0.5, 0.5,

	-0.5,-0.5,-0.5,
	-0.5,-0.5, 0.5,
	 0.5,-0.5, 0.5,
	 0.5,-0.5,-0.5,

	-0.5, 0.5,-0.5,
	-0.5, 0.5, 0.5,
	 0.5, 0.5, 0.5,
	 0.5, 0.5,-0.5,

	-0.5,-0.5,-0.5,
	-0.5,-0.5, 0.5,
	-0.5, 0.5, 0.5,
	-0.5, 0.5,-0.5,

	 0.5,-0.5,-0.5,
	 0.5,-0.5, 0.5,
	 0.5, 0.5, 0.5,
	 0.5, 0.5,-0.5
};

float cubeTexCoords[] = {
	0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.0, 0.5,
	0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.0, 0.5,
	0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.0, 0.5,
	0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.0, 0.5,
	0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.0, 0.5,
	0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.0, 0.5
};

float cubeNormals[] = {
	0.0, 0.0 , 0.5, 0.0, 0.0, 0.5, 0.0, 0.0, 0.5, 0.0, 0.0, 0.5
};

float cubeColors[] = {
	0.5, 0.0, 0.0,  0.5, 0.0, 0.0,  0.5, 0.0, 0.0,  0.5, 0.0, 0.0,
	0.0, 0.5, 0.0,  0.0, 0.5, 0.0,  0.0, 0.5, 0.0,  0.0, 0.5, 0.0,
	0.0, 0.0, 0.5,  0.0, 0.0, 0.5,  0.0, 0.0, 0.5,  0.0, 0.0, 0.5,
	0.5, 0.5, 0.0,  0.5, 0.5, 0.0,  0.5, 0.5, 0.0,  0.5, 0.5, 0.0,
	0.0, 0.5, 0.5,  0.0, 0.5, 0.5,  0.0, 0.5, 0.5,  0.0, 0.5, 0.5,
	0.5, 0.5, 0.5,  0.5, 0.5, 0.5,  0.5, 0.5, 0.5,  0.5, 0.5, 0.5
};



#endif
