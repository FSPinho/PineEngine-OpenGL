
#version 330 core

uniform sampler2D COLOR;

in vec4 position;
out vec4 fragmentOutColor;

void main() {
    fragmentOutColor = texture(COLOR, position.xy * 0.5 + vec2(0.5, 0.5));
}
