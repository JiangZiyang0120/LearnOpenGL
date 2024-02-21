//
// Created by jasperyang on 24-2-21.
//

#include <Renderer.h>
#include <VertexArray.h>


VertexArray::VertexArray() {
    GLCall(glGenVertexArrays(1, &m_RendererID));
    GLCall(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray() {
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::setLayout(const VertexBuffer &VBO, const VertexBufferLayout &layout) {
    VBO.bind();

    const auto &elements = layout.getElements();
    GLulong offset = 0;
    for (size_t i = 0; i != elements.size(); ++i) {
        const auto &element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized,
                                     layout.getStride(), (const void *) offset));
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}

void VertexArray::bind() const {
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::unbind() const {
    GLCall(glBindVertexArray(0));
}
