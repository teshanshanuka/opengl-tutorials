#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>
#include <iostream>

#define ASSERT(x) \
    if (!(x)) throw std::runtime_error(std::string("Assertion error: ") + #x)

#define GLCall(x)    \
    GLClearErrors(); \
    x;               \
    ASSERT(GLCheckErrors(__FILE__, #x, __LINE__))

static bool GLCheckErrors(const char* file, const char* function, int line) {
    // GL_INVALID_ENUM; // Uncomment ant ctrl click to goto error codes
    while (GLenum err = glGetError()) {
        std::cerr << "[OpenGL Err] " << err << " " << file << ":" << line << " in " << function << "\n";
        return false;
    }
    return true;
}

static void GLClearErrors() {
    while (glGetError() != GL_NO_ERROR);
}
