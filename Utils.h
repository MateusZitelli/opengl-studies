#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <GL/glew.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

static const double PI = 3.14159265358979323846;

typedef struct Vector
{
    float values[4];
} Vector;

typedef struct Vertex
{
    float Position[4];
    float Color[4];
    float Normal[4];
} Vertex;

typedef struct Matrix
{
    float m[16];
} Matrix;

extern const Matrix IDENTITY_MATRIX;

float Cotangent(float angle);
float DegreesToRadians(float degrees);
float RadiansToDegrees(float radians);

Matrix MultiplyMatrices(const Matrix* m1, const Matrix* m2);

void RotateAboutX(Matrix* m, float angle);
void RotateAboutY(Matrix* m, float angle);
void RotateAboutZ(Matrix* m, float angle);
void ScaleMatrix(Matrix* m, float x, float y, float z);
void TranslateMatrix(Matrix* m, float x, float y, float z);

void PrintMatrix(Matrix m);

Matrix CreateCameraMatrix(
    Vector position,
    Vector lookat,
    Vector orientation
);

Matrix CreateProjectionMatrix(
    float fovy,
    float aspect_ratio,
    float near_plane,
    float far_plane
);

void ExitOnGLError(const char* error_message);
void IgnoreOnGLError(const char* error_message);
#endif
