#version 330 core
layout (location = 0) in vec3 vertexInPosition;
layout (location = 1) in vec4 vertexInColor;

out vec4 vertexOutColor;

uniform float TIME;
uniform mat4 TRANSFORM;

void main() {
    vec3 offset = vec3(0.0f, sin(TIME * 2.0) * 0.2, 0.0f);
    vec4 position = TRANSFORM * vec4(vertexInPosition + offset, 1.0);
    gl_Position = position;
    vertexOutColor = vertexInColor;
}