#version 330 core
layout (location = 0) in vec4 vertexInPosition;
layout (location = 1) in vec4 vertexInNormal;
layout (location = 2) in vec4 vertexInLightInfluence;

out vec4 vertexOutNormal;
out float vertexOutLightInfluence[16];
out vec4 vertexOutWorldPos;

uniform mat4 MODEL_MATRIX;
uniform mat4 VIEW_MATRIX;
uniform mat4 PROJECTION_MATRIX;

void main() {
    vec4 position = vertexInPosition;
    vec4 normal = vertexInNormal;

    vertexOutNormal = MODEL_MATRIX * normal;
    vertexOutWorldPos = MODEL_MATRIX * position;

    for (int i = 0; i < 16; i++) {
        bool isLightEnabled = (int(vertexInLightInfluence[0]) & (1 << i)) != 0;
        vertexOutLightInfluence[i] = isLightEnabled ? 1.0f : 0.0f;
    }

    gl_Position = PROJECTION_MATRIX * VIEW_MATRIX * MODEL_MATRIX * position;
}