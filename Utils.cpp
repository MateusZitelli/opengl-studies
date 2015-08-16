#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include "Utils.h"

using namespace std;

const Matrix IDENTITY_MATRIX = {{
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1
}};

float Cotangent(float angle){
    return (float)(1.0 / tan(angle));
}

float DegreesToRadians(float degrees){
    return degrees * (float)(PI / 180);
}

float RadiansToDeegrees(float radians){
    return radians * (float) (180 / PI);
}

Matrix MultiplyMatrices(const Matrix* m1, const Matrix* m2)
{
  Matrix out = IDENTITY_MATRIX;
  unsigned int row, column, row_offset;
 
  for (row = 0, row_offset = row * 4; row < 4; ++row, row_offset = row * 4)
    for (column = 0; column < 4; ++column)
      out.m[row_offset + column] =
        (m1->m[row_offset + 0] * m2->m[column + 0]) +
        (m1->m[row_offset + 1] * m2->m[column + 4]) +
        (m1->m[row_offset + 2] * m2->m[column + 8]) +
        (m1->m[row_offset + 3] * m2->m[column + 12]);
 
  return out;
}

void ScaleMatrix(Matrix* m, float x, float y, float z){
    Matrix scale = IDENTITY_MATRIX;
    scale.m[0] = x;
    scale.m[5] = y;
    scale.m[10] = z;
    memcpy(m->m, MultiplyMatrices(m, &scale).m, sizeof(m->m));
}

void TranslateMatrix(Matrix* m, float x, float y, float z)
{
  Matrix translation = IDENTITY_MATRIX;
  
  translation.m[12] = x;
  translation.m[13] = y;
  translation.m[14] = z;
 
  memcpy(m->m, MultiplyMatrices(m, &translation).m, sizeof(m->m));
}


void RotateAboutX(Matrix* m, float angle)
{
  Matrix rotation = IDENTITY_MATRIX;
  float sine = (float)sin(angle);
  float cosine = (float)cos(angle);
  
  rotation.m[5] = cosine;
  rotation.m[6] = -sine;
  rotation.m[9] = sine;
  rotation.m[10] = cosine;
 
  memcpy(m->m, MultiplyMatrices(m, &rotation).m, sizeof(m->m));
}
 
void RotateAboutY(Matrix* m, float angle)
{
  Matrix rotation = IDENTITY_MATRIX;
  float sine = (float)sin(angle);
  float cosine = (float)cos(angle);
  
  rotation.m[0] = cosine;
  rotation.m[8] = sine;
  rotation.m[2] = -sine;
  rotation.m[10] = cosine;
 
  memcpy(m->m, MultiplyMatrices(m, &rotation).m, sizeof(m->m));
}
 
void RotateAboutZ(Matrix* m, float angle)
{
  Matrix rotation = IDENTITY_MATRIX;
  float sine = (float)sin(angle);
  float cosine = (float)cos(angle);
  
  rotation.m[0] = cosine;
  rotation.m[1] = -sine;
  rotation.m[4] = sine;
  rotation.m[5] = cosine;
 
  memcpy(m->m, MultiplyMatrices(m, &rotation).m, sizeof(m->m));
}
 
Matrix CreateProjectionMatrix(
  float fovy,
  float aspect_ratio,
  float near_plane,
  float far_plane
)
{
  Matrix out = { { 0 } };
 
  const float
    y_scale = Cotangent(DegreesToRadians(fovy / 2)),
    x_scale = y_scale / aspect_ratio,
    frustum_length = far_plane - near_plane;
 
  out.m[0] = x_scale;
  out.m[5] = y_scale;
  out.m[10] = -((far_plane + near_plane) / frustum_length);
  out.m[11] = -1;
  out.m[14] = -((2 * near_plane * far_plane) / frustum_length);
  
  return out;
}
 
Matrix CreateCameraMatrix(
    Vector position,
    Vector lookat,
    Vector orientation
){
    float diffPLa[3], N[3], u[3], U[3], V[3];
    float diffPLaSize = 0, uSize = 0;
    // diffPLa = Position - LookAt
    for(int i = 0; i < 3; i++){
        diffPLa[i] = position.values[i] - lookat.values[i];
        diffPLaSize += diffPLa[i] * diffPLa[i];
    }

    // |diffPLa|
    diffPLaSize = sqrt(diffPLaSize);

    // N = dffPLa / |diffPLa|
    for(int i = 0; i < 3; i++){
        N[i] = diffPLa[i] / diffPLaSize;
    }
    
    // u = Orientation X N
    for(int i = 0; i < 3; i++){
        int id0 = (i + 2) % 3, id1 = (i + 1) % 3;
        u[i] = N[id0] * orientation.values[id1] - N[id1] * orientation.values[id0];
        uSize += u[i] * u[i];
    }

    uSize = sqrt(uSize);

    // U = u / |u|
    for(int i = 0; i < 3; i++){
        U[i] = u[i] / uSize;
    }

    // V = N X U
    for(int i = 0; i < 3; i++){
        int id0 = (i + 2) % 3, id1 = (i + 1) % 3;
        V[i] = N[id1] * U[id0] - N[id0] * U[id1];
    }

    float UdotPosition = 0, VdotPosition = 0, NdotPosition = 0;

    for(int i = 0; i < 3; i++){
        UdotPosition += U[i] * position.values[i];
        VdotPosition += V[i] * position.values[i];
        NdotPosition += N[i] * position.values[i];
    }

    Matrix out = { 
        {
            U[0], V[0], N[0], 0,
            U[1], V[1], N[1], 0,
            U[2], V[2], N[2], 0,
            - UdotPosition, - VdotPosition, - NdotPosition, 1
        }
    };

    return out;
}

void PrintMatrix(const Matrix M){
    for(int i = 0; i < 16; i++){
        printf("%f ", M.m[i]);
        if(i % 4 == 3){
            printf("\n");
        }
    }
}

void ExitOnGLError(const char* error_message)
{
  const GLenum ErrorValue = glGetError();
 
  if (ErrorValue != GL_NO_ERROR)
  {
    fprintf(stderr, "%s: %s\n", error_message, gluErrorString(ErrorValue));
    exit(EXIT_FAILURE);
  }
}
 
void IgnoreOnGLError(const char* error_message)
{
  const GLenum ErrorValue = glGetError();
 
  if (ErrorValue != GL_NO_ERROR)
  {
    fprintf(stderr, "%s: %s\n", error_message, gluErrorString(ErrorValue));
  }
}

string readFile(const char* filename){
    ifstream file (filename);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
