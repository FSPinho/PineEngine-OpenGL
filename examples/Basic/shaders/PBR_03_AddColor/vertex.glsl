#version 330 core
layout (location = 0) in vec4 vertexInPosition;

void main() {
    gl_Position = vertexInPosition;
}