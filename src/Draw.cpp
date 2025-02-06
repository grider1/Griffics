#include "Draw.h"
#include "Object3D.h"
#include <iostream>
#include <vector>
#include <utility>

std::vector<std::pair<vec3, vec3>> getTriEdges(const triangle& tri) {
  std::vector<std::pair<vec3, vec3>> edges;
  std::pair<vec3, vec3> edge0 = std::make_pair(tri.p[0], tri.p[1]);
  std::pair<vec3, vec3> edge1 = std::make_pair(tri.p[0], tri.p[2]);
  std::pair<vec3, vec3> edge2 = std::make_pair(tri.p[1], tri.p[2]);
  
  edges.push_back(edge0);
  edges.push_back(edge1);
  edges.push_back(edge2);

  return edges;
} 

void drawTri(SDL_Renderer* renderer, const triangle& tri) {

  SDL_SetRenderDrawColor(renderer,255,255,255,255);
  for (int i = 0; i < 3; i++) {
    SDL_RenderDrawPoint(renderer, tri.p[i].x, tri.p[i].y);
  }

  std::vector<std::pair<vec3, vec3>> edges = getTriEdges(tri);

  for (int i = 0; i < edges.size(); ++i) {
//    std::cout << edges[i].first.x << edges[i].first.y << edges[i].second.x << edges[i].second.y;
    SDL_RenderDrawLine(renderer, edges[i].first.x, edges[i].first.y, edges[i].second.x, edges[i].second.y);
  }

}

void drawMesh(SDL_Renderer* renderer, mesh &imesh) {
  for (size_t i = 0; i < imesh.tris.size(); ++i) {
    drawTri(renderer, imesh.tris[i]);
  }
}

void drawScene(SDL_Renderer* renderer, std::vector<mesh> &scene) {
  for (size_t i = 0; i < scene.size(); ++i) {
    drawMesh(renderer, scene[i]);
  }
}
