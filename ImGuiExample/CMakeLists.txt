cmake_minimum_required(VERSION 3.22)
project(abstract)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 3.3 REQUIRED)

set(OPENGL_LIBRARY /usr/lib/x86_64-linux-gnu/libGL.so)
set(GLEW_LIBRARY /usr/lib/x86_64-linux-gnu/libGLEW.so)
set(IMGUI_LIBRARY /usr/lib/x86_64-linux-gnu/libimgui.a)
set(STB_LIBRARY /usr/lib/x86_64-linux-gnu/libstb.so)

include_directories(${PROJECT_SOURCE_DIR}/../include
        ${PROJECT_SOURCE_DIR}/../external_include
        /usr/include/imgui
        /usr/include/imgui/backends)

file(GLOB project_file main.cpp)

aux_source_directory(${PROJECT_SOURCE_DIR}/../src src_file)

add_executable(${PROJECT_NAME} ${src_file} ${project_file})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY} ${GLEW_LIBRARY} ${IMGUI_LIBRARY} ${STB_LIBRARY} glfw stb)