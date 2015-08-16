#include "World.h"

void World::init(int programId){
    projectionMatrixUniformLocation = glGetUniformLocation(programId, "ProjectionMatrix");
    ExitOnGLError("ERROR: Could not get the projection uniform locations");

    camera.init(programId);
    light.init(programId);

    obj.programId = programId;
    glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, projectionMatrix.m);
}

void World::updateProjection(int width, int height){
    setProjectionMatrix(width, height);
    glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, projectionMatrix.m);
}

void World::tick(){
    camera.tick();
    light.tick();
    obj.draw();
}

void World::clean(){
    obj.remove();
}

void World::setProjectionMatrix(int width, int height){
    projectionMatrix = CreateProjectionMatrix(
        60.0f,
        (float) width / height,
        0.5f,
        1000.0f
    );
}
