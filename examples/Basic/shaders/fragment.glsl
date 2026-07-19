#version 330 core

out vec4 fragmentOutColor;
in vec4 vertexOutColor;

void main() {
    fragmentOutColor = vertexOutColor;
}