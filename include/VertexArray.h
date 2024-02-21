#ifndef UNIFORM_VERTEX_ARRAY_H
#define UNIFORM_VERTEX_ARRAY_H

#include <VertexBuffer.h>
#include <VertexBufferLayout.h>

class VertexArray {
public:
    VertexArray();

    ~VertexArray();

    void setLayout(const VertexBuffer &VBO, const VertexBufferLayout &layout);

    void bind() const;

    void unbind() const;

private:
    GLuint m_RendererID;
};


#endif //UNIFORM_VERTEX_ARRAY_H
