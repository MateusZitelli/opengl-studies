#include <iostream>
#include "Utils.h"
#include "Engine.h"

using namespace std;

float rotation = 0;
clock_t LastTime = 0;

void CreateCube(void);
void DestroyCube(void);
void DrawCube(void);

GLuint ProjectionMatrixUniformLocation,
       ModelMatrixUniformLocation,
       LightPositionsUniformLocation,
       BufferIds[3] = { 0 };

Matrix ViewMatrix,
       ModelMatrix;

int lastTime = 0;
static Object* obj;
static Camera* camera;
static Light* light;

void static frameCallback(){
    float rotation, angle;
    clock_t Now = clock();
    if(LastTime == 0)
        LastTime = Now;

    rotation += 90.0f * ((float) (Now - LastTime) / CLOCKS_PER_SEC);
    angle = DegreesToRadians(rotation);
    light->setPosition(sin(angle / 7.0) * 10, cos(angle / 7.0) * 10, 0);
    camera->setPosition(sin(angle) * 7, 5, cos(angle) * 7); 
}

int main(void){
    int width = 800, height = 600;
    Engine engine;
    engine.init(width, height);

    obj = &engine.world.obj;
    obj->load("models/beetle.off");
    obj->scale(5, 5, 5);

    camera = &engine.world.camera;
    light = &engine.world.light;

    camera->lookAt(0, 0, 0);
    camera->setOrientation(0, 1, 0);

    engine.renderLoop(frameCallback);
    engine.clean();
}

