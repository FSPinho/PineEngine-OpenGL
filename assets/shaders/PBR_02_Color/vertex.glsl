#version 330 core
layout (location = 0) in vec4 vertexInPosition;
layout (location = 1) in vec4 vertexInNormal;

uniform float TIME;
uniform mat4 MODEL_MATRIX;
uniform mat4 VIEW_MATRIX;
uniform mat4 PROJECTION_MATRIX;

out vec4 position;
out vec4 normal;

void main() {
    position = MODEL_MATRIX * vertexInPosition;
    normal = MODEL_MATRIX * vertexInNormal;

    float distortion1 = sin(TIME * 4.0f + position.y * 2.0) * 0.1;
    float distortion2 = sin(TIME * 0.5f) * 0.5;
    position.x += distortion1 * distortion2;

    gl_Position = PROJECTION_MATRIX * VIEW_MATRIX * position;
}