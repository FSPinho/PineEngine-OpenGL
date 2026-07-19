#version 330 core

in vec4 vertexOutColor;
in vec4 vertexOutNormal;
out vec4 fragmentOutColor;

void main() {
    vec3 lightDirection = vec3(0.0f, 4.0f, 0.5f);
    float lightIntensity = dot(lightDirection, vertexOutNormal.xyz) * 0.5 + 0.5;

    fragmentOutColor = vec4(vec3(1.0f, 1.0f, 1.0f) * lightIntensity, 1.0f);
}