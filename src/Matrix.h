#ifndef MATRIX_h
#define MATRIX_h
#include "Object3D.h"

struct matrix4x4 {
  float m[4][4];
};

void vecMultiply(vec3 &i, vec3 &o, matrix4x4 m);
mesh transformMesh(mesh &imesh, matrix4x4 &transformMatrix);
mesh rotateMesh(mesh &imesh, float angle);
void offsetAndProjectMesh(mesh &imesh, float zOffset, matrix4x4 projMatrix);

#endif
