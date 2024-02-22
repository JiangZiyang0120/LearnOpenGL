//
// Created by jasperyang on 24-2-21.
//

#include <Tool.h>
#include <Renderer.h>
#include <GL/glew.h>

void Renderer::draw(const VertexArray &VAO, const IndexBuffer &IBO, const Shader &shader) const {
    shader.bind();
    VAO.bind();
    IBO.bind();

    GLCall(glDrawElements(GL_TRIANGLES, IBO.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::clear() const {
    glClear(GL_COLOR_BUFFER_BIT);
}
