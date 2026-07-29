#version 330 core

uniform sampler2D HDRI;

in vec4 position;
layout(location = 0) out vec4 colorBlur0;
layout(location = 1) out vec4 colorBlur1;

const vec2 invAtan = vec2(0.1591, 0.3183);
const float PI = 3.1415;

vec2 sampleSphericalMap(vec3 v);

void main() {
    colorBlur0 = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    colorBlur1 = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    vec3 N = normalize(position.xyz);
    vec3 upRef = vec3(0.0f, 1.0f, 0.0f);
    if (dot(N, upRef) > 0.9999) {
        upRef = vec3(1.0f, 0.0f, 0.0f);
    }
    vec3 T = normalize(cross(N, upRef));
    vec3 B = cross(N, T);

    colorBlur0.xyz += texture(HDRI, sampleSphericalMap(N)).xyz;

    float step = PI / 24.0f;
    float itCount = 0.0f;
    for (float lat = step; lat <= PI / 2.0f; lat += step) {
        for (float lng = step; lng <= PI * 2.0f; lng += step) {
            float x = cos(lat) * cos(lng);
            float y = sin(lat);
            float z = cos(lat) * sin(lng);

            vec3 dir1 = T * x + B * y + N * z;
            colorBlur1.xyz += clamp(texture(HDRI, sampleSphericalMap(dir1)).xyz, vec3(0.0f), vec3(1.0f));

            itCount += 1.0f;
        }
    }

    colorBlur1.xyz /= itCount;
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