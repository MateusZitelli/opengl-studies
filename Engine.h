#ifndef ENGINE_H
#define ENGINE_H
#include "Configs.h"
#include "Utils.h"
#include "World.h"
#include <vector>

class Engine{
    static World* activeWorld;

    public:
        Engine();
        virtual ~Engine(){}
        GLFWwindow* window;
        World world;
        void init(int width, int height);
        void renderLoop(void (*frameCallback)(void));
        void clean();
    private:
        GLuint loadShader(const char* filename, GLenum shaderType);
        GLuint createProgram(GLuint vertexShader, GLuint fragmentShader);
        void loadProgram();
        static void errorCallback(int, const char*);
        static void windowSizeCallback(GLFWwindow* window, int width, int height);
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        void initializeGL(int width, int height);
        void genWindow(int width, int height);
        void destroy();

#ifdef GEOMETRY_SHADER
        GLuint ShaderIds[4] = { 0 };
#else
        GLuint ShaderIds[3] = { 0 };
#endif
};
#endif
