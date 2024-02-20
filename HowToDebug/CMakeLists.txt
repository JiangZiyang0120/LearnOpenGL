cmake_minimum_required(VERSION 3.22)
project(debug)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 3.3 REQUIRED)

set(OPENGL_LIBRARY /usr/lib/x86_64-linux-gnu/libGL.so)
set(GLEW_LIBRARY /usr/lib/x86_64-linux-gnu/libGLEW.so)

file(GLOB project_file main.cpp)
add_executable(${PROJECT_NAME} ${project_file})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY} ${GLEW_LIBRARY} glfw)