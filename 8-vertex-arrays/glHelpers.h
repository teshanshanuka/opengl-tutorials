#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "utils.h"

using std::string;

#define ASSERT(x) \
    if (!(x)) throw std::runtime_error(string("Assertion error: ") + #x)
#define GLCall(x)    \
    GLClearErrors(); \
    x;               \
    ASSERT(GLCheckErrors(__FILE__, #x, __LINE__))

static void GLClearErrors() {
    while (glGetError() != GL_NO_ERROR);
}

static bool GLCheckErrors(const char* file, const char* function, int line) {
    // GL_INVALID_ENUM; // Uncomment ant ctrl click to goto error codes
    while (GLenum err = glGetError()) {
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

void glStatus() {
    if (glewInit() != GLEW_OK) std::cout << "glew init failed!" << std::endl;
    std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;
}
