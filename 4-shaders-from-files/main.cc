#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using std::string;

struct ShaderProgSource {
    string vertex_src;
    string fragment_src;
};

static ShaderProgSource parseShader(const string& file_path) {
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

static int CreateShader(const string& vertexShader, const string& fragmentShader) {
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
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    status();

    float positions[6] = {
        -0.5f, -0.5f, 0.0f, 0.5f, 0.5f, -0.5f,
    };
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    ShaderProgSource srcs = parseShader("/home/teshan/code/opengl-play/4-shaders-from-files/res/shaders/basic.shader");
    uint shader = CreateShader(srcs.vertex_src, srcs.fragment_src);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Clean up
    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}