#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "Utils.h"
#include "Object.h"

#define FRAGMENT_SHADER "shaders/phong.frag.sdr"
#define VERTEX_SHADER "shaders/phong.vert.sdr"
//#define FRAGMENT_SHADER "shaders/normals.frag.sdr"
//#define VERTEX_SHADER "shaders/normals.vert.sdr"
//#define GEOMETRY_SHADER "shaders/normals.geo.sdr"

#define CULLING false

using namespace std;

float CubeRotation = 0;
clock_t LastTime = 0;

void CreateCube(void);
void DestroyCube(void);
void DrawCube(void);

GLuint ProjectionMatrixUniformLocation,
       ViewMatrixUniformLocation,
       ModelMatrixUniformLocation,
       BufferIds[3] = { 0 },
#ifdef GEOMETRY_SHADER
       ShaderIds[4] = { 0 };
#else
       ShaderIds[3] = { 0 };
#endif

Matrix ProjectionMatrix,
       ViewMatrix,
       ModelMatrix;

string readFile(const char* filename){
    ifstream file (filename);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void error_callback(int error, const char* description){
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

static void window_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    ProjectionMatrix = CreateProjectionMatrix(
        60.0f,
        (float) width / height,
        0.5f,
        1000.0f
    );

    glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, ProjectionMatrix.m);
    ExitOnGLError("ERROR: Could not set the shader uniforms");
}

GLuint load_shader(const char* filename, GLenum shaderType){
    GLint Result = GL_FALSE;
    int InfoLogLength;
    string shaderFileString = readFile(filename);
    const char* shaderText = shaderFileString.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderText, NULL);
    glCompileShader(shader);

    ExitOnGLError("ERROR: Cound not load shader");
    // Check fragment shader
    glGetShaderiv(shader, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
    vector<char> ErrorMessage(InfoLogLength);
    glGetShaderInfoLog(shader, InfoLogLength, NULL, &ErrorMessage[0]);
    fprintf(stdout, "Shader log: %s\n", &ErrorMessage[0]);
    return shader;
}

 GLuint load_program(GLuint vertexShader, GLuint fragmentShader){
    GLint Result = GL_FALSE;
    int InfoLogLength;

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    ExitOnGLError("ERROR: Could not load attach shaders to program");
    // Check the program
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &Result);
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(shaderProgram, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "Shader program log: %s\n", &ProgramErrorMessage[0]);
    return shaderProgram;
}

GLFWwindow* GenWindow(int width, int height){
    glfwSetErrorCallback(error_callback);

    if(!glfwInit())
        exit(EXIT_FAILURE);

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(width, height, "GL tests", NULL,NULL);

    if(!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    ExitOnGLError("ERROR: could not load window");
    return window;
}

void LoadProgram(){
    ShaderIds[1] = load_shader(VERTEX_SHADER, GL_VERTEX_SHADER);
    ShaderIds[2] = load_shader(FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
    ShaderIds[0] = load_program(ShaderIds[1], ShaderIds[2]);
    #ifdef GEOMETRY_SHADER
    ShaderIds[3] = load_shader(GEOMETRY_SHADER, GL_GEOMETRY_SHADER);
    glAttachShader(ShaderIds[0], ShaderIds[3]);
    #endif

    glLinkProgram(ShaderIds[0]);
    ExitOnGLError("ERROR: Could not link the shader program");
    glUseProgram(ShaderIds[0]);
}

void InitializeGL(int width, int height){
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        glfwTerminate();
        exit(EXIT_FAILURE); 
    }
    IgnoreOnGLError("Error: Glew error in init");

    glDepthMask(GL_TRUE); 
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    ExitOnGLError("Error: Could not set OpenGL depth testing options");
   
#if CULLING
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    ExitOnGLError("Error: Could not set OpenGL culling options");
#endif

    ProjectionMatrix = CreateProjectionMatrix(
        60.0f,
        (float) width / height,
        0.5f,
        1000.0f
    );
}

void LoadWorldUniformsLocations(void){
    LoadProgram();    
    ViewMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ViewMatrix");
    ProjectionMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ProjectionMatrix");
    ExitOnGLError("ERROR: Could not get the shader uniform locations");
}

void DestroyWorld(void){
    glDetachShader(ShaderIds[0], ShaderIds[1]);
    glDetachShader(ShaderIds[0], ShaderIds[2]);
    glDeleteShader(ShaderIds[1]);
    glDeleteShader(ShaderIds[2]);
    glDeleteProgram(ShaderIds[0]);
    ExitOnGLError("Error: Could not destroy shaders");
}

void SubmitWorldUniforms(){
    float CubeAngle;
    ExitOnGLError("ERROR: Could not use the shader program");
    clock_t Now = clock();
    if(LastTime == 0)
        LastTime = Now;

    CubeRotation += 90.0f * ((float) (Now - LastTime) / CLOCKS_PER_SEC);

    CubeAngle = DegreesToRadians(CubeRotation);

    LastTime = Now;
    // RotateAboutX(&ModelMatrix, CubeAngle);
    // RotateAboutY(&ModelMatrix, CubeAngle);
    // TranslateMatrix(&ModelMatrix, sin(CubeAngle) * 2, cos(CubeAngle) * 2, sin(CubeAngle * 2) * 2);
    const Vector position = {{4 * sin(CubeAngle), 3, 4 * cos(CubeAngle)}},
        lookat = {{0, 0, 0}},
        orientation = {{0, 1, 0}};

    ViewMatrix = CreateCameraMatrix(position, lookat, orientation);

    glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE, ViewMatrix.m);
    glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, ProjectionMatrix.m);
    ExitOnGLError("ERROR: Could not set the shader uniforms");
}

int main(void){
    Object obj;
    int width = 800, height = 600;
    GLFWwindow* window = GenWindow(width, height);
    InitializeGL(width, height);
    LoadWorldUniformsLocations();
    obj.load("models/beetle.off", ShaderIds[0]);
    obj.scale(3, 3, 3);
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SubmitWorldUniforms();
        obj.draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    obj.remove();
    DestroyWorld();
    glfwTerminate();
}

