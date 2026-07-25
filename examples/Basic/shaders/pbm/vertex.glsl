#version 330 core
layout (location = 0) in vec3 vertexInPosition;
layout (location = 1) in vec3 vertexInNormal;
layout (location = 2) in float vertexInLightInfluence;

out vec4 vertexOutNormal;
out float vertexOutLightInfluence[16];
out vec4 vertexOutWorldPos;

uniform mat4 MODEL_MATRIX;
uniform mat4 VIEW_MATRIX;
uniform mat4 PROJECTION_MATRIX;

void main() {
    vec4 position = vec4(vertexInPosition, 1.0);
    vec4 normal = vec4(vertexInNormal, 0.0);

    vertexOutNormal = MODEL_MATRIX * normal;
    vertexOutWorldPos = MODEL_MATRIX * position;

    for (int i = 0; i < 16; i++) {
        bool isLightEnabled = (int(vertexInLightInfluence) & (1 << i)) != 0;
        vertexOutLightInfluence[i] = isLightEnabled ? 1.0f : 0.0f;
    }

    gl_Position = PROJECTION_MATRIX * VIEW_MATRIX * MODEL_MATRIX * position;
}