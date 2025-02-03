#ifndef OBJECT2D_H
#define OBJECT2D_H

#include <vector>

struct Vector2 {
  float x, y;

  Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

struct Object2D {
  std::vector<Vector2> points;

  Object2D(const std::vector<Vector2>& points) : points(points) {}

  size_t getPointCount() const {
    return points.size();
  }
};

#endif
