#version 330 core
layout (location = 0) in vec4 vertexInPosition;
layout (location = 1) in vec4 vertexInNormal;

uniform mat4 MODEL_MATRIX;
uniform mat4 VIEW_MATRIX;
uniform mat4 PROJECTION_MATRIX;

out vec4 position;
out vec4 normal;

void main() {
    position = MODEL_MATRIX * vertexInPosition;
    normal = MODEL_MATRIX * vertexInNormal;
    gl_Position = PROJECTION_MATRIX * VIEW_MATRIX * position;
}