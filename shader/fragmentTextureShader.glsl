#version 330 core

layout(location=0) out vec4 color;

in vec2 v_TexCoords;

uniform sampler2D u_Texture;

void main() {
    vec4 texColor = texture(u_Texture, v_TexCoords);
    color = texColor;
}
