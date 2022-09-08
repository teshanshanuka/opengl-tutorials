#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "utils.h"

using std::string;

#define ASSERT(x) if (!x) throw std::runtime_error(string("Assertion error: ") + #x)
#define GLCall(x) GLClearErrors();\
    x;\
    ASSERT(GLCheckErrors(__FILE__, #x, __LINE__))

static void GLClearErrors(){ 
    while(glGetError() != GL_NO_ERROR);
}

static bool GLCheckErrors(const char* file, const char* function, int line){
    while(GLenum err = glGetError()) {
        std::cerr << "[OpenGL Err] " << err << " " << file << ":" << line << " in " << function << "\n";
        return false;
    }
    return true;
}

struct ShaderProgSource {
    string vertex_src;
    string fragment_src;
};

static ShaderProgSource parseShader(const string& file_path) {
    utils::asserFileExists(file_path);

    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

    ShaderType type(ShaderType::NONE);
    std::ifstream stream(file_path);
    std::stringstream ss[2];
    string line;

    while (std::getline(stream, line)) {
        if (line.find("#shader") != string::npos) {
            if (line.find("vertex") != string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)type] << line << "\n";
        }
    }
    return {ss[0].str(), ss[1].str()};
}

static uint compileShader(uint type, const string& src) {
    uint id = glCreateShader(type);
    const char* srcp = src.c_str();
    glShaderSource(id, 1, &srcp, nullptr);
    glCompileShader(id);

    // Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        char* msg = (char*)alloca(len * sizeof(char));
        glGetShaderInfoLog(id, len, &len, msg);

        std::cout << "Error: shader-" << src << " err-" << msg << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static int createShader(const string& vertexShader, const string& fragmentShader) {
    uint prog = glCreateProgram();
    uint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    uint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    glValidateProgram(prog);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return prog;
}

void status() {
    if (glewInit() != GLEW_OK) {
        std::cout << "glew init failed!" << std::endl;
    }
    std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;
}

int main(void) {
    /* ************** INIT ************** */
    GLFWwindow* window;
    if (!glfwInit()) return -1;
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    status();

    /* ************** STUFF ************** */
    float positions[] = {
        -0.5f, -0.5f, 
        0.5f, -0.5f, 
        0.5f, 0.5f,
        -0.5f, 0.5f,
    };
    // I want the to make two triangle out of these
    uint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    uint buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    uint ibo;  // Index buffer obj
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint), indices, GL_STATIC_DRAW));

    ShaderProgSource srcs = parseShader(utils::getFilePath(__FILE__) + "/res/shaders/basic.shader");
    uint shader = createShader(srcs.vertex_src, srcs.fragment_src);
    GLCall(glUseProgram(shader));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);  -> correct
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr));

        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    // Clean up
    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}