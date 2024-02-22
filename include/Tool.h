//
// Created by jasperyang on 24-2-22.
//

#ifndef ABSTRACT_TOOL_H
#define ABSTRACT_TOOL_H

#include <cstdlib>

#define GLCheckError(x) GLCheckErrorWithLine(__FILE__, #x, __LINE__)

#define GLCall(x) GLClearError(); \
                  x;              \
                  GLCheckError(x)

#define ASSERT(x) if(x) showMessage(__FILE__, __LINE__), abort()

void showMessage(const char *file, int line);

void GLClearError();

void GLCheckErrorWithLine(const char *file, const char *function, int line);
#endif //ABSTRACT_TOOL_H
