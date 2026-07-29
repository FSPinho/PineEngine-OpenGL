#version 330 core
layout (location = 0) in vec4 vertexInPosition;

uniform mat4 MODEL_MATRIX;
uniform mat4 VIEW_MATRIX;
uniform mat4 PROJECTION_MATRIX;

void main() {
    gl_Position = PROJECTION_MATRIX * VIEW_MATRIX * MODEL_MATRIX * vertexInPosition;
}