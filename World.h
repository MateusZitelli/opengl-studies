#ifndef WORLD_H
#define WORLD_H
#include "Utils.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"

class World{
    public:
        World() {}
        virtual ~World() {}

        Object obj;
        Camera camera;
        Light light;

        void setProjectionMatrix(int width, int height);
        void updateProjection(int width, int height);
        void init(int programId);
        void render();
        void tick();
        void clean();
    private:
        Matrix projectionMatrix;
        GLuint projectionMatrixUniformLocation;
};
#endif
