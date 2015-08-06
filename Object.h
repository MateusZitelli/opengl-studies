#include <Utils.h>

typedef unsigned short indexT;

class Object{
    public:
        Object() {}
        virtual ~Object() {}
        void load(const char *filename, GLuint ShaderId);
        void draw();
        void print();
        void remove();
    protected:
    private:
        int nv, nf, ne;

        Vertex *vert;
        indexT *ind;
        GLuint BufferIds[3];
        GLuint ShaderId;
        GLuint ModelMatrixUniformLocation;
        void prepare();
};
