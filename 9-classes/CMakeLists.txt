cmake_minimum_required(VERSION 3.10)
project(opengl-play)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)

add_library(indexBuf
    indexBuffer.cc
    indexBuffer.hh
    renderer.hh
)

add_library(vertexBuf
    vertexBuffer.cc
    vertexBuffer.hh
    renderer.hh
)

add_executable(main
    main.cc
    renderer.hh
    glHelpers.h
    utils.h
)
target_link_libraries(main
    indexBuf
    vertexBuf
    glfw
    OpenGL::GL
    GLEW
)
