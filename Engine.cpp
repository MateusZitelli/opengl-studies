#include "Engine.h"
#include "World.h"
using namespace std;

World * aw;

Engine::Engine(){
    aw = &world;
}

GLuint Engine::loadShader(const char* filename, GLenum shaderType){
    GLint Result = GL_FALSE;
    int InfoLogLength;
    string shaderFileString = readFile(filename);
    const char* shaderText = shaderFileString.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderText, NULL);
    glCompileShader(shader);

    ExitOnGLError("ERROR: Cound not load shader");
    // Check the shader
    glGetShaderiv(shader, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
    vector<char> ErrorMessage(InfoLogLength);
    glGetShaderInfoLog(shader, InfoLogLength, NULL, &ErrorMessage[0]);
    fprintf(stdout, "Shader log: %s\n", &ErrorMessage[0]);
    return shader;
}


GLuint Engine::createProgram(GLuint vertexShader, GLuint fragmentShader){
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

void Engine::loadProgram(){
    ShaderIds[1] = loadShader(VERTEX_SHADER, GL_VERTEX_SHADER);
    ShaderIds[2] = loadShader(FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
    ShaderIds[0] = createProgram(ShaderIds[1], ShaderIds[2]);
    #ifdef GEOMETRY_SHADER
    ShaderIds[3] = loadShader(GEOMETRY_SHADER, GL_GEOMETRY_SHADER);
    glAttachShader(ShaderIds[0], ShaderIds[3]);
    #endif

    glLinkProgram(ShaderIds[0]);
    ExitOnGLError("ERROR: Could not link the shader program");
    glUseProgram(ShaderIds[0]);
    ExitOnGLError("ERROR: Could not use the shader program");
}

void Engine::errorCallback(int error, const char* description){
    fputs(description, stderr);
}

void Engine::windowSizeCallback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    aw->updateProjection(width, height);
}

void Engine::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void Engine::initializeGL(int width, int height){
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

    world.setProjectionMatrix(width, height);
}


void Engine::genWindow(int width, int height){
    glfwSetErrorCallback(this->errorCallback);

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
    glfwSetKeyCallback(window, this->keyCallback);
    glfwSetWindowSizeCallback(window, this->windowSizeCallback);
    ExitOnGLError("ERROR: could not load window");
    this->window = window;
}

void Engine::renderLoop(void (*frameCallback)(void)){
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        frameCallback();
        world.tick();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Engine::destroy(){
    glDetachShader(ShaderIds[0], ShaderIds[1]);
    glDetachShader(ShaderIds[0], ShaderIds[2]);
    glDeleteShader(ShaderIds[1]);
    glDeleteShader(ShaderIds[2]);
    glDeleteProgram(ShaderIds[0]);
    ExitOnGLError("Error: Could not destroy shaders");
}

void Engine::clean(){
    world.clean();
    destroy();
    glfwTerminate();
}

void Engine::init(int width, int height){
    genWindow(width, height);
    initializeGL(width, height);
    loadProgram();
    world.init(ShaderIds[0]);
}
