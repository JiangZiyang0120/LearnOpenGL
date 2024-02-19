cmake_minimum_required(VERSION 3.22)
project(creatingWindow)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 3.3 REQUIRED)

set(OPENGL_LIBRARIES /usr/lib/x86_64-linux-gnu/libGL.so)

include_directories(${PROJECT_SOURCE_DIR}/../include/)
include_directories(${OPENGL_INCLUDE_DIRS})
link_directories(${OPENGL_LIBRARY_DIRS})

aux_source_directory(${PROJECT_SOURCE_DIR}/../src/ src_file)

file(GLOB project_file main.cpp)
add_executable(${PROJECT_NAME} ${src_file} ${project_file})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw)