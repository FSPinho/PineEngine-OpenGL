#version 330 core

in vec4 vertexOutNormal;
in float vertexOutLightInfluence[16];
in vec4 vertexOutWorldPos;

out vec4 fragmentOutColor;

uniform PointLight POINT_LIGHTS[16];
uniform DirectionalLight DIRECTIONAL_LIGHTS[16];
uniform int POINT_LIGHTS_COUNT;
uniform int DIRECTIONAL_LIGHTS_COUNT;
uniform vec3 VIEW_POSITION;

const float PI = 3.14159265359;
const float GAMMA_CORRECTION = 1.0f / 2.2f;

vec3 Lo(
        vec3 L_vec,
        vec3 Li,
        vec3 albedo,
        float roughness,
        vec3 reflectance,
        float metallic
);
vec3 Li_PointLight(vec3 I, float L_r);
float GGX_Trowbridge_Reitz(float roughness, float N_dot_H);
float Schlick_GGZ(float roughness, float N_dot_V, float N_dot_L);
vec3 fresnel(vec3 F0, float V_dot_H);
vec3 ACES(vec3 x);

void main() {
    vec3 albedo = vec3(1.0f, 1.0f, 1.0f);
    vec3 outColor = vec3(0.0f, 0.0f, 0.0f);
    float roughness = 0.5;
    vec3 reflectance = vec3(0.04);
    float metallic = 0.0;
    float expousure = 1.0; // / pow(2, 9.75);

    for (int i = 0; i < POINT_LIGHTS_COUNT; i++) {
        vec3 I = POINT_LIGHTS[i].radiantIntensity;
        vec3 L_vec = POINT_LIGHTS[i].translation - vertexOutWorldPos.xyz;
        float L_r = length(L_vec);
        vec3 Li = Li_PointLight(I, L_r);

        outColor += Lo(L_vec, Li, albedo, roughness, reflectance, metallic) * vertexOutLightInfluence[i];
    }

    for (int i = 0; i < DIRECTIONAL_LIGHTS_COUNT; i++) {
        vec3 L_vec = DIRECTIONAL_LIGHTS[i].direction;
        vec3 I = DIRECTIONAL_LIGHTS[i].irradiance;
        vec3 Li = I;
        outColor += Lo(L_vec, Li, albedo, roughness, reflectance, metallic);
    }

    outColor = ACES(outColor * expousure);
    fragmentOutColor = vec4(pow(outColor, vec3(GAMMA_CORRECTION)), 1.0f);
}

vec3 Lo(
        vec3 L_vec,
        vec3 Li,
        vec3 albedo,
        float roughness,
        vec3 reflectance,
        float metallic
) {
    vec3 L = normalize(L_vec);

    vec3 V = normalize(VIEW_POSITION - vertexOutWorldPos.xyz);
    vec3 N = vertexOutNormal.xyz;
    vec3 H = normalize(L + V);
    float epsilon = 0.000001;

    // ...
    float N_dot_L = max(dot(N, L), 0.0);
    float N_dot_V = max(dot(N, V), 0.0);
    float N_dot_H = max(dot(N, H), 0.0);
    float V_dot_H = max(dot(V, H), 0.0);

    // ...
    float D = GGX_Trowbridge_Reitz(roughness, N_dot_H);
    float G = Schlick_GGZ(roughness, N_dot_V, N_dot_L);
    vec3 F = fresnel(reflectance, V_dot_H);

    // Specular
    vec3 specular = (D * F * G) / (4.0f * N_dot_L * N_dot_V + epsilon);

    // Diffuse
    vec3 diffuse = (1.0f - metallic) * albedo / PI;

    return (diffuse + specular) * Li * N_dot_L;
}

vec3 Li_PointLight(vec3 I, float L_r) {
    return I / pow(L_r, 2.0f);
}

float GGX_Trowbridge_Reitz(float roughness, float N_dot_H) {
    float a = roughness * roughness;
    float a2 = a * a;
    float denom = N_dot_H * N_dot_H * (a2 - 1.0f) + 1.0f;
    return a2 / (PI * denom * denom);
}

float Schlick_GGZ(float roughness, float N_dot_V, float N_dot_L) {
    float k = roughness + 1.0f;
    k = (k * k) / 8.0;

    float Gv = N_dot_V / (N_dot_V * (1.0f - k) + k);
    float G1 = N_dot_L / (N_dot_L * (1.0f - k) + k);

    return Gv * G1;
}

vec3 fresnel(vec3 F0, float V_dot_H) {
    return F0 + (1.0f - F0) * pow(1.0f - V_dot_H, 5.0);
}

vec3 ACES(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}