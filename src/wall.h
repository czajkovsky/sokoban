#ifndef WALL_H
#define WALL_H

int wallVertexCount = 4;

float wallVertices[] = {
  -0.5,-0.5, 0.5,
  -0.5, 0.5, 0.5,
   0.5, 0.5, 0.5,
   0.5,-0.5, 0.5,
};

float wallTexCoords[] = {
  0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.0, 0.5,
};

float wallColors[] = {
  0.5, 0.0, 0.0,  0.5, 0.0, 0.0,  0.5, 0.0, 0.0,  0.5, 0.0, 0.0,
};

#endif
