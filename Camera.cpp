#include "Camera.h"

void Camera::init(GLuint programId){
    matrixUniformLocation = glGetUniformLocation(programId, "ViewMatrix");
    ExitOnGLError("ERROR: Could not get the camera uniform locations");
}

void Camera::tick(){
    Matrix ViewMatrix = CreateCameraMatrix(position, lookat, orientation);
    glUniformMatrix4fv(matrixUniformLocation, 1, GL_FALSE, ViewMatrix.m);
}

void Camera::setPosition(float x, float y, float z){
    position = {{x, y, z}};
}

void Camera::lookAt(float x, float y, float z){
    lookat = {{x, y, z}};
}

void Camera::setOrientation(float x, float y, float z){
    orientation = {{x, y, z}};
}
