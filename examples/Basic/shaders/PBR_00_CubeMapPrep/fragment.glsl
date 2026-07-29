#version 330 core

uniform sampler2D HDRI;

in vec4 position;
out vec4 color;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 sampleSphericalMap(vec3 v);

void main() {
    color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec2 uv = sampleSphericalMap(normalize(position.xyz));
    color.xyz += texture(HDRI, uv).xyz;
}

vec2 sampleSphericalMap(vec3 v) {
    vec2 uv = vec2(
            atan(v.z, v.x),
            asin(v.y)
    );
    uv *= invAtan;
    uv += 0.5;
    uv.y = 1.0 - uv.y;
    return uv;
}