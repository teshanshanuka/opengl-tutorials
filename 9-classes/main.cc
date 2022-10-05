#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

#include "renderer.hh"
#include "indexBuffer.hh"
#include "vertexBuffer.hh"
#include "glHelpers.h"
// #include "utils.h"

using std::string;

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

    glfwSwapInterval(1);

    glStatus();

    /* ************** STUFF ************** */
    float positions[] = {
        -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,
    };
    // I want the to make two triangle out of these
    uint indices[] = {0, 1, 2, 2, 3, 0};

    // vertex array object
    uint vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    VertexBuffer vb(positions, 4*2*sizeof(float));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    IndexBuffer ib(indices, 6);

    ShaderProgSource srcs = parseShader(utils::getFilePath(__FILE__) + "/res/shaders/basic.shader");
    uint shader = createShader(srcs.vertex_src, srcs.fragment_src);

    // Deal with the uniform
    int location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1);

    // Unbind everything
    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    float r = .0f, inc = .05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        if (r > 1)
            inc = -0.05f;
        else if (r < 0)
            inc = 0.05f;
        r += inc;

        GLCall(glUseProgram(shader));
        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

        GLCall(glBindVertexArray(vao));
        ib.bind();

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

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
