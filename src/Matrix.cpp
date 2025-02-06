#include "Matrix.h"

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

void offsetAndProjectMesh(mesh &imesh, float xOffset, float yOffset, float zOffset, matrix4x4 projMatrix) {
  // Take in mesh and transform it into projected mesh. (screen space mesh)
  for (int i = 0; i < imesh.tris.size(); i++) {
    triangle projTri;
    for (int j = 0; j < 3; j++) {
      vec3 transVec3 = imesh.tris[i].p[j];
      transVec3.x += xOffset;
      transVec3.y += yOffset;
      transVec3.z += zOffset;
      vec3 projVec3 = {0.0f, 0.0f, 0.0f};
      vecMultiply(transVec3, projVec3, projMatrix);
      projVec3.x += 1.0f; projVec3.y += 1.0f;
      projVec3.x *= 0.5f * 2560.0f;
      projVec3.y *= 0.5f * 1440.0f;
      projTri.p[j] = projVec3;
    }
    imesh.tris[i] = projTri;
  }
}
