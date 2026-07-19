#version 330 core
layout (location = 0) in vec3 vertexInPosition;
layout (location = 1) in vec3 vertexInNormal;

out vec4 vertexOutColor;
out vec4 vertexOutNormal;

uniform mat4 MODEL_MATRIX;
uniform mat4 VIEW_MATRIX;
uniform mat4 PROJECTION_MATRIX;

void main() {
    vec4 position = vec4(vertexInPosition, 1.0);
    vec4 normal = vec4(vertexInNormal, 0.0);

    vertexOutColor = position;
    vertexOutNormal = normal;

    gl_Position = PROJECTION_MATRIX * VIEW_MATRIX * MODEL_MATRIX * position;
}