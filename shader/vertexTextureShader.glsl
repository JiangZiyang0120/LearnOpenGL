#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec2 texCoords;

out vec2 v_TexCoords;

void main() {
    gl_Position = position;
    gl_Position.y = -gl_Position.y;
    v_TexCoords = texCoords;
}
