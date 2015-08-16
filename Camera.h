#ifndef CAMERA_H
#define CAMERA_H
#include "Utils.h"

class Camera{
    public:
        Camera(){}
        void init(GLuint programId);
        void tick();
        void setPosition(float x, float y, float z);
        void lookAt(float x, float y, float z);
        void setOrientation(float x, float y, float z);
    private:
        GLuint matrixUniformLocation;
        Vector position;
        Vector lookat;
        Vector orientation;
};
#endif
