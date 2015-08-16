#ifndef LIGHT_H
#define LIGHT_H
#include "Utils.h"

class Light{
    public:
        Light(){}
        void init(int programId);
        void tick();
        void setPosition(float x, float y, float z);
    private:
        GLuint uniformLocation;
        Vector position;
        Vector lookat;
        Vector orientation;
};
#endif
