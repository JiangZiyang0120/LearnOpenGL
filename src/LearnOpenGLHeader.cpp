//
// Created by jasperyang on 24-2-21.
//

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include "LearnOpenGLHeader.h"

void showMessage(const char *file, int line){
    std::cerr << "File " << file << " Line " << line << std::endl;
}

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

void GLCheckErrorWithLine(const char *file, const char *function, int line) {
    while (GLenum error = glGetError()) {
        std::cerr << "File " << file << "\nLine " << line << " " << function << "\nOpenGL Error: (" << error << ")"
                  << std::endl;
    }
}

std::string getPath(const std::string &str) {
    return currentFilepath + str;
}

std::string getShader(const std::string &&filePath) {
    std::ifstream file(getPath(filePath));
    if (file.is_open()) {
        std::string content((std::istreambuf_iterator<char>(file)),
                            (std::istreambuf_iterator<char>()));
        return content;
    } else {
        return "Error Occurred";
    }
}