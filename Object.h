#ifndef OBJECT_H
#define OBJECT_H
#include "Utils.h"

typedef unsigned short indexT;

class Object{
    public:
        Object() {}
        virtual ~Object() {}
        void load(const char *filename);
        void draw();
        void print();
        void remove();
        void scale(float x, float y, float z);
        void translate(float x, float y, float z);
        void rotateAboutX(float angle);
        void rotateAboutY(float angle);
        void rotateAboutZ(float angle);
        GLuint programId;
    protected:
    private:
        int nv, nf, ne;
        Vertex *vert;
        indexT *ind;
        Matrix ModelMatrix;
        GLuint BufferIds[3];
        GLuint ModelMatrixUniformLocation;
        void prepare();
};
#endif
