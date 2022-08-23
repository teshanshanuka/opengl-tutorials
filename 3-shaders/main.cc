#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

using std::string;

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
        char* msg = (char*)alloca(len*sizeof(char));
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

    string vertexShader = 
        "#version 330 core\n"
        ""
        "layout(location = 0) in vec4 position;\n"
        ""
        "void main(){\n"
        "   gl_Position = position;\n"
        "}";

    string fragmentShader = 
        "#version 330 core\n"
        ""
        "layout(location = 0) out vec4 color;\n"
        ""
        "void main(){\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}";

    uint shader = createShader(vertexShader, fragmentShader);
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