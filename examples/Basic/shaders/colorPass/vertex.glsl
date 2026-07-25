#version 330 core
layout (location = 0) in vec4 vertexInPosition;
layout (location = 1) in vec4 vertexInNormal;

out vec4 position;

void main() {
    position = vertexInPosition;
    gl_Position = vertexInPosition;
}