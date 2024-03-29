#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec2 texCoords;

out vec2 v_TexCoords;

// model view projection
uniform mat4 u_MVP;

void main() {
    gl_Position = u_MVP * position;
    v_TexCoords = texCoords;
}
