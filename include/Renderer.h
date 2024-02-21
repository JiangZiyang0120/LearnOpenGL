//
// Created by jasperyang on 24-2-21.
//

#ifndef UNIFORM_RENDERER_H
#define UNIFORM_RENDERER_H

#include <GL/glew.h>

#include <string>

#define GLCheckError(x) GLCheckErrorWithLine(__FILE__, #x, __LINE__)

#define GLCall(x) GLClearError(); \
                  x;              \
                  GLCheckError(x)

#define ASSERT(x) if(x) showMessage(__FILE__, __LINE__), abort()

void showMessage(const char *file, int line);

void GLClearError();

void GLCheckErrorWithLine(const char *file, const char *function, int line);

static const std::string currentFilepath = "../../Shader/";

#endif //UNIFORM_RENDERER_H
