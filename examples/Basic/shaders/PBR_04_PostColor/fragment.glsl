#version 330 core

// Uniforms
uniform sampler2D COLOR;
uniform float EXPOSURE;

// In/out
out vec4 color;

// Constants
const float GAMMA_CORRECTION = 1.0f / 2.2f;

vec3 ACES(vec3 x);

void main() {
    color = texelFetch(COLOR, ivec2(gl_FragCoord.xy), 0);
    color.xyz *= EXPOSURE;
    color.xyz = ACES(color.xyz);
    color.xyz = pow(color.xyz, vec3(GAMMA_CORRECTION));
}

vec3 ACES(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}