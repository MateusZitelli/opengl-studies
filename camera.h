#ifndef CAMERA_H
#define CAMERA_H
#include <array>
#include <cmath>
#include <cstdio>

using namespace std;

class camera {
    public:
        camera();
        const float *mptr;
        virtual ~camera();
        void define(float xp, float yp, float zp,float xla,float yla,float zla,float vx,float vy,float vz, float zpf);
        void print();

    protected:
    private:
        array <float, 16> m;


        float xp, yp, zp, xla, yla, zla, vcx, vcy, vcz, zpf;



};



void camera::define(float xp, float yp, float zp,float xla,float yla,float zla,float vcx,float vcy,float vcz, float zpf) {
    // Definir a matriz da câmera e depois já multiplicá-la pela matriz de projeção.  Use o array m



}


void camera::print() {
    printf("Camera Matrix\n");
    printf("%4.1f %4.1f %4.1f %4.1f\n", m[0], m[1], m[2], m[3]);
    printf("%4.1f %4.1f %4.1f %4.1f\n", m[4], m[5], m[6], m[7]);
    printf("%4.1f %4.1f %4.1f %4.1f\n", m[8], m[9], m[10], m[11]);
    printf("%4.1f %4.1f %4.1f %4.1f\n\n", m[12], m[13], m[14], m[15]);
}


camera::camera() {
    m = {1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1};
    mptr = m.data();
}

camera::~camera() {
    //dtor
}


#endif // CAMERA_H
