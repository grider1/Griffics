#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include "Object2D.h"
#include "Object3D.h"
#include "Matrix.h"
#include "Draw.h"

void vecMultiply(vec3 &i, vec3 &o, matrix4x4 m) {
  o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
  o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
  o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
  float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];
  if(w != 0.0f) {
    o.x /= w; o.y /= w; o.z /= w;
  }
}

mesh transformMesh(mesh &imesh, matrix4x4 &transformMatrix) {
  mesh transMesh;
  for (int i = 0; i < imesh.tris.size(); i++) {
    triangle transTri;
    for (int j = 0; j < 3; j++) {
      vec3 transVec3 = {0.0f, 0.0f, 0.0f};
      vecMultiply(imesh.tris[i].p[j], transVec3, transformMatrix);
      transTri.p[j] = transVec3;
    }
    transMesh.tris.push_back(transTri);
  }
  return transMesh;
}

mesh rotateMesh(mesh &imesh, float angle) {
  matrix4x4 matRotZ, matRotX;
  float fTheta = angle;

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
  matrix4x4 matRot;
  // Combine the rotation matrices

  mesh xmesh = transformMesh(imesh, matRotX);
  return transformMesh(xmesh, matRotX);
}

int main(int argc, const char * argv[]) {
  
  SDL_DisplayMode DM;
  SDL_GetCurrentDisplayMode(0, &DM);
  int Width = 2560;
  int Height = 1440;

  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(640*4,480*4, 0, &window, &renderer);
  //SDL_RenderSetScale(renderer,4,4);
  
  SDL_SetRenderDrawColor(renderer,0,0,0,255);
  SDL_RenderClear(renderer);
  
  mesh meshCube = cube();

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

  for (int i = 0; i < meshCube.tris.size(); i++) {
    triangle projTri;
    for (int j = 0; j < 3; j++) {
      vec3 transVec3 = meshCube.tris[i].p[j];
      vec3 projVec3 = {0.0f, 0.0f, 0.0f};
      vecMultiply(transVec3, projVec3, matProj);
      projTri.p[j] = projVec3;
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

