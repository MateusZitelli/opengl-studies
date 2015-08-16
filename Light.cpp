#include "Light.h"

void Light::init(int programId){
    uniformLocation = glGetUniformLocation(programId, "LightPosition");
    ExitOnGLError("ERROR: Could not get the light uniform locations");
}

void Light::tick(){
    glUniform4f(uniformLocation, position.values[0], position.values[1], position.values[2], position.values[3]);
}

void Light::setPosition(float x, float y, float z){
    position = {{x, y, z}};
}
