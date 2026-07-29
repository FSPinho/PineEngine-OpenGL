#version 330 core

// Uniforms
uniform sampler2DMS COLOR;

// In/out
out vec4 color;

void main() {
    color = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; i++) {
        color += texelFetch(COLOR, ivec2(gl_FragCoord.xy), i);
    }

    color /= 4.0;
}