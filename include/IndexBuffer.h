//
// Created by jasperyang on 24-2-21.
//

#ifndef UNIFORM_INDEXBUFFER_H
#define UNIFORM_INDEXBUFFER_H

#include <GL/glew.h>

class IndexBuffer {
public:
    IndexBuffer(const GLuint *data, unsigned int count);

    ~IndexBuffer();

    void bind() const;

    void unbind() const;

    inline GLuint getCount() const { return m_Count; }

private:
    GLuint m_RenderID;
    // we need to know how many indices are
    // m_Count and count is the number of elements in the indices set
    GLuint m_Count;
};


#endif //UNIFORM_INDEXBUFFER_H
