#version 330 core

uniform samplerCube CUBE;

in vec4 position;
out vec4 color;

void main() {
   vec3 uv = normalize(position.xyz);
   color = vec4(texture(CUBE, uv).xyz, 1.0);
}
