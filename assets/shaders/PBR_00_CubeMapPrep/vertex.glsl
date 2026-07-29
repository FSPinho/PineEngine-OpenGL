#version 330 core
layout (location = 0) in vec4 vertexInPosition;

out vec4 position;

uniform mat4 VIEW_MATRIX;
uniform mat4 PROJECTION_MATRIX;

void main() {
    position = vertexInPosition;
    gl_Position = PROJECTION_MATRIX * VIEW_MATRIX * vertexInPosition;
}