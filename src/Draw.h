#ifndef DRAW_H
#define DRAW_H
#include <SDL2/SDL.h>
#include "Object2D.h"
#include "Object3D.h"

void drawTri(SDL_Renderer* renderer, const triangle& tri);

std::vector<std::pair<vec3, vec3>> getTriEdges(const triangle& tri);

#endif
