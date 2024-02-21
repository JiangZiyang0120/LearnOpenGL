//
// Created by jasperyang on 24-2-21.
//

#ifndef UNIFORM_VERTEXBUFFER_H
#define UNIFORM_VERTEXBUFFER_H

#include <GL/glew.h>

class VertexBuffer {
public:
    VertexBuffer(const void *data, GLuint size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

private:
    GLuint m_RenderID;
};

#endif //UNIFORM_VERTEXBUFFER_H
