#version 330 core

// Uniforms
uniform sampler2D ENVIRONMENT;
uniform sampler2D COLOR;
uniform float EXPOSURE;

// In/out
out vec4 color;

// Constants
const float GAMMA_CORRECTION = 1.0f / 2.2f;

vec3 ACES(vec3 x);

void main() {
    vec4 fgColor = texelFetch(COLOR, ivec2(gl_FragCoord.xy), 0);
    fgColor.xyz *= EXPOSURE;
    fgColor.xyz = ACES(fgColor.xyz);
    fgColor.xyz = pow(fgColor.xyz, vec3(GAMMA_CORRECTION));
    fgColor.a = clamp(fgColor.a, 0.0f, 1.0f);

    vec4 envColor = texelFetch(ENVIRONMENT, ivec2(gl_FragCoord.xy), 0);
    envColor.xyz = ACES(envColor.xyz);
    envColor.xyz = pow(envColor.xyz, vec3(GAMMA_CORRECTION));

    color.xyz = envColor.xyz * (1.0f - fgColor.a) + fgColor.xyz * fgColor.a;
    color.a = 1.0f;
}

vec3 ACES(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}