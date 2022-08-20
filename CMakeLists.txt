cmake_minimum_required(VERSION 3.10)
project(opengl-play)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_executable(main
    main.cc
)
target_link_libraries(main
    glfw
    OpenGL::GL
)