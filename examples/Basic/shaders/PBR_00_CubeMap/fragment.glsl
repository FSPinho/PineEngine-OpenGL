#version 330 core

uniform sampler2D HDRI;

in vec3 position;
out vec4 color;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 sampleSphericalMap(vec4 v);

void main() {
    vec2 uv = sampleSphericalMap(normalize(position));
    color = texture(HDRI, uv);
}

vec2 sampleSphericalMap(vec4 v) {
    vec2 uv = vec2(
            atan(v.z, v.x),
            asin(v.y)
    );
    uv *= invAtan;
    uv += 0.5;
    return uv;
}