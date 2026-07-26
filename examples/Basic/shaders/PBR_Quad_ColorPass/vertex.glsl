#version 330 core
layout (location = 0) in vec4 vertexInPosition;
layout (location = 1) in vec4 vertexInNormal;

out vec2 uv;

void main() {
    uv = vertexInPosition.xy * 0.5 + vec2(0.5, 0.5);
    gl_Position = vertexInPosition;
}