#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <vector>

struct vec3 {
  float x, y, z;
};

struct triangle {
  vec3 p[3];
};

struct mesh {
  std::vector<triangle> tris;
};

mesh cube();

#endif
