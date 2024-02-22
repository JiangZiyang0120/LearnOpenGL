//
// Created by jasperyang on 24-2-21.
//

#ifndef UNIFORM_VERTEX_BUFFER_LAYOUT_H
#define UNIFORM_VERTEX_BUFFER_LAYOUT_H

#include <Tool.h>
#include <vector>
#include <GL/glew.h>

struct VertexBufferElement {
    GLuint count;
    GLenum type;
    GLboolean normalized;

    static GLuint getSizeOfType(GLenum type) {
        switch (type) {
            case GL_FLOAT:
                return sizeof(GLfloat);
            case GL_UNSIGNED_INT:
                return sizeof(GLuint);
            case GL_UNSIGNED_BYTE:
                return sizeof(GLbyte);
            default:
                ASSERT(true);
        }
    }
};

class VertexBufferLayout {
public:
    VertexBufferLayout() : m_Stride(0) {}

    template<typename T>
    void push(GLuint count) {
        ASSERT(true);
    }

    [[nodiscard]] std::vector<VertexBufferElement> getElements() const { return m_Elements; }

    [[nodiscard]] GLuint getStride() const { return m_Stride; }

private:
    std::vector<VertexBufferElement> m_Elements;
    GLuint m_Stride;
};

/*
 * Class member template functions are not allowed to be defined within a class. However, if its source file references
 * the header file, the compiler generates two specialization functions inside the header file and the source file. This
 * results in multiple definition errors.
 *
 * To avoid this situation, you can consider:
 * 1. Using the `inline` keyword to declare and define the class member template function. By placing the specialized
 *    template function definition in the header file and using inline for declaration and definition, you can prevent
 *    multiple definition errors.
 * 2. Moving the implementation of the specialized template function to the source file and only provide a declaration
 *    in the header file, which also helps to avoid multiple definition errors.
 */

template<>
inline void VertexBufferLayout::push<GLfloat>(GLuint count) {
    m_Elements.push_back({count, GL_FLOAT, GL_FALSE});
    m_Stride += count * sizeof(GLfloat);
}

template<>
inline void VertexBufferLayout::push<GLuint>(GLuint count) {
    m_Elements.push_back({count, GL_UNSIGNED_INT, GL_FALSE});
    m_Stride += count * sizeof(GLuint);
}

template<>
inline void VertexBufferLayout::push<GLubyte>(GLuint count) {
    m_Elements.push_back({count, GL_UNSIGNED_BYTE, GL_TRUE});
    m_Stride += count * sizeof(GLubyte);
}


#endif //UNIFORM_VERTEX_BUFFER_LAYOUT_H
