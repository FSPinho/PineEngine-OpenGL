#version 430

struct PointLight {
    vec3 translation;
    vec3 radiantIntensity;
};
struct DirectionalLight {
    vec3 direction;
    vec3 irradiance;
};
uniform PointLight POINT_LIGHTS[16];
uniform DirectionalLight DIRECTIONAL_LIGHTS[16];
uniform int POINT_LIGHTS_COUNT;
uniform int DIRECTIONAL_LIGHTS_COUNT;

uniform float X_OFFSET;
uniform float Y_OFFSET;

layout(local_size_x = 1, local_size_y = 1) in;

struct Vertex {
    vec4 vertexInPosition;
    vec4 vertexInNormal;
    vec4 vertexInLightInfluence;
};
struct Index {
    uint index;
};

layout(std430, binding = 0) coherent buffer VertexBuffer {
    Vertex vertices[];
};
layout(std430, binding = 1) coherent buffer IndexBuffer {
    Index indices[];
};

void main() {
    uint flags = 0xFFFF;
    uint x = gl_GlobalInvocationID.x + uint(Y_OFFSET);

    for (uint li = 0; li < POINT_LIGHTS_COUNT; li++) {
        vec3 lightPosition = POINT_LIGHTS[li].translation;
        vec3 v = lightPosition - vertices[x].vertexInPosition.xyz;

        if (dot(normalize(v), vertices[x].vertexInNormal.xyz) < 0) continue;

        uint index = gl_GlobalInvocationID.y + uint(Y_OFFSET);

        if (
            indices[index * 3].index == x ||
            indices[index * 3 + 1].index == x ||
            indices[index * 3 + 2].index == x
        ) {
            continue;
        }

        const vec3 v1 = lightPosition - vec3(vertices[indices[index * 3 + 0].index].vertexInPosition);
        const vec3 v2 = lightPosition - vec3(vertices[indices[index * 3 + 1].index].vertexInPosition);
        const vec3 v3 = lightPosition - vec3(vertices[indices[index * 3 + 2].index].vertexInPosition);

        const vec3 v1v = normalize(normalize(v1) - normalize(v));
        const vec3 v2v = normalize(normalize(v2) - normalize(v));
        const vec3 v3v = normalize(normalize(v3) - normalize(v));

        const float dot1 = dot(v1v, v2v);
        const float dot2 = dot(v2v, v3v);
        const float dot3 = dot(v3v, v1v);

        const uint sum = uint(dot1 < 0) + uint(dot2 < 0) + uint(dot3 < 0);

        if (sum >= 2) {
            flags &= ~(1u << (li / 3u));
            break;
        }
    }

    vertices[x].vertexInLightInfluence[0] = flags;
}