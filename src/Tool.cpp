//
// Created by jasperyang on 24-2-22.
//

#include <Tool.h>
#include <GL/glew.h>
#include <iostream>

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