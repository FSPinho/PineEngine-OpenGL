#version 330 core

in vec4 position;
out vec4 fragmentOutColor;

void main() {
    fragmentOutColor = position;
}