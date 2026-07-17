#version 330 core
layout (location = 0) in vec3 vertexInPosition;
layout (location = 1) in vec4 vertexInColor;

out vec4 vertexOutColor;

uniform float TIME;

void main() {
    vec3 offset = vec3(0.0f, sin(TIME * 2.0) * 0.2, 0.0f);
    gl_Position = vec4(vertexInPosition + offset, 1.0);
    vertexOutColor = vertexInColor;
}