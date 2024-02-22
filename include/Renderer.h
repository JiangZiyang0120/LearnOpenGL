//
// Created by jasperyang on 24-2-21.
//

#ifndef UNIFORM_RENDERER_H
#define UNIFORM_RENDERER_H

#include <IndexBuffer.h>
#include <VertexArray.h>
#include <Shader.h>

class Renderer {
public:
    void clear() const;

    void draw(const VertexArray &VAO, const IndexBuffer &IBO, const Shader &shader) const;
};

#endif //UNIFORM_RENDERER_H
