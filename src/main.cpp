#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include "Object2D.h"
#include "Object3D.h"
#include "Matrix.h"
#include "Draw.h"

std::vector<mesh> createWorld() {
  int Width = 2560;
  int Height = 1440;

  float fNear = 0.1f;
  float fFar = 1000.0f;
  float fFov = 90.0f;
  float fAspectRatio = (float)Height / (float)Width;
  float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
  matrix4x4 matProj;
  matProj.m[0][0] = fAspectRatio * fFovRad;
  matProj.m[1][1] = fFovRad;
  matProj.m[2][2] = fFar / (fFar - fNear);
  matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
  matProj.m[2][3] = 1.0f;
  matProj.m[3][3] = 0.0f;
  std::vector<mesh> world = {};
  int LENGTH = 10;
  int WIDTH = 10;

  for (int i = 0; i < 1; ++i) {
    for (int j = 0; j < 1; ++j) {
      mesh meshCube = cube();
      offsetAndProjectMesh(meshCube, 0.0f, 0.0f, 3.0f, matProj);
      world.push_back(meshCube);
    }
  }
  return world;
}

int main(int argc, const char * argv[]) {
  
  // Create and initialize window
  SDL_DisplayMode DM;
  SDL_GetCurrentDisplayMode(0, &DM);
  int Width = 2560;
  int Height = 1440;

  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(640*4,480*4, 0, &window, &renderer);
  // Clear screen with black
  SDL_SetRenderDrawColor(renderer,0,0,0,255);
  SDL_RenderClear(renderer);
 
  // Create cube
  mesh meshCube = cube();

  // Define view parameters -- used for Projection matrix
  float fNear = 0.1f;
  float fFar = 1000.0f;
  float fFov = 90.0f;
  float fAspectRatio = (float)Height / (float)Width;
  float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
  matrix4x4 matProj;
  matProj.m[0][0] = fAspectRatio * fFovRad;
  matProj.m[1][1] = fFovRad;
  matProj.m[2][2] = fFar / (fFar - fNear);
  matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
  matProj.m[2][3] = 1.0f;
  matProj.m[3][3] = 0.0f;
  
  // Create Rotation matrices
  matrix4x4 matRotZ, matRotX;
  float fTheta = 1;
  // Rotation Z
  matRotZ.m[0][0] = cosf(fTheta);
  matRotZ.m[0][1] = sinf(fTheta);
  matRotZ.m[1][0] = -sinf(fTheta);
  matRotZ.m[1][1] = cosf(fTheta);
  matRotZ.m[2][2] = 1;
  matRotZ.m[3][3] = 1;
  // Rotation X
  matRotX.m[0][0] = 1;
  matRotX.m[1][1] = cosf(fTheta * 0.5f);
  matRotX.m[1][2] = sinf(fTheta * 0.5f);
  matRotX.m[2][1] = -sinf(fTheta * 0.5f);
  matRotX.m[2][2] = cosf(fTheta * 0.5f);
  matRotX.m[3][3] = 1; 

  // Take in mesh and transform it into projected mesh. (screen space mesh)
  //offsetAndProjectMesh(meshCube, 0.0f, 0.0f, 3.0f, matProj);
  //std::vector<mesh> scene = {};
  //scene.push_back(meshCube);

  std::vector<mesh> world = {};
  int LENGTH = 10;
  int WIDTH = 10;

  for (int i = 0; i < LENGTH; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      mesh meshCube = cube();
      offsetAndProjectMesh(meshCube, j - 3.0f, 2.0f, i, matProj);
      world.push_back(meshCube);
    }
  }
  //drawMesh(renderer, meshCube);
  drawScene(renderer, world);

  float count = 0.01f;
  SDL_RenderPresent(renderer); //render here

  SDL_Event e;
  bool quit = false;
  while(!quit){
    while (SDL_PollEvent(&e)){
          if (e.type == SDL_QUIT){
              quit = true;
          }
          if (e.type == SDL_KEYDOWN){

            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            SDL_RenderClear(renderer);
            // Rotation Z
            matRotZ.m[0][0] = cosf(count);
            matRotZ.m[0][1] = sinf(count);
            matRotZ.m[1][0] = -sinf(count);
            matRotZ.m[1][1] = cosf(count);
            matRotZ.m[2][2] = 1;
            matRotZ.m[3][3] = 1;

            // Rotation X
            matRotX.m[0][0] = 1;
            matRotX.m[1][1] = cosf(count * 0.5f);
            matRotX.m[1][2] = sinf(count * 0.5f);
            matRotX.m[2][1] = -sinf(count * 0.5f);
            matRotX.m[2][2] = cosf(count * 0.5f);
            matRotX.m[3][3] = 1; 

             meshCube = cube();
              for (int i = 0; i < meshCube.tris.size(); i++) {
              triangle projTri;
              for (int j = 0; j < 3; j++) {
                vec3 transVec3 = meshCube.tris[i].p[j];
                vec3 projVec3 = {0.0f, 0.0f, 0.0f};
                vec3 finalVec3 = {0.0f, 0.0f, 0.0f};
                vecMultiply(transVec3, projVec3, matRotZ);
                vecMultiply(projVec3, finalVec3, matRotX);
                projTri.p[j] = finalVec3;
              }
              meshCube.tris[i] = projTri;
            }

            for (int i = 0; i < meshCube.tris.size(); i++) {
              triangle projTri;
              for (int j = 0; j < 3; j++) {
                vec3 transVec3 = meshCube.tris[i].p[j];
                transVec3.z += 3.0f;
                vec3 projVec3 = {0.0f, 0.0f, 0.0f};
                vecMultiply(transVec3, projVec3, matProj);
                projVec3.x += 1.0f; projVec3.y += 1.0f;
                projVec3.x *= 0.5f * 2560.0f;
                projVec3.y *= 0.5f * 1440.0f;
                projTri.p[j] = projVec3;
              }
              drawTri(renderer, projTri);
            }
            SDL_RenderPresent(renderer); //render here
            count += 0.1f;
          }
          if (e.type == SDL_MOUSEBUTTONDOWN){
              quit = true;
          }
      }
  }

  return 0;

}

