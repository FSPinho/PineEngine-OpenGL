#version 330 core

// In/out
in vec4 position;
in vec4 normal;
out vec4 color;

// Uniforms
struct DirectionalLight {
    uint enabled;
    vec3 direction;
    vec3 irradiance;
    uint enableShadows;
    uint enableSSAO;
};
uniform DirectionalLight DIRECTIONAL_LIGHT;

struct PointLight {
    uint enabled;
    vec3 translation;
    vec3 radiantIntensity;
    uint enableShadows;
    uint enableSSAO;
};
uniform PointLight POINT_LIGHT;

uniform vec3 VIEW_POSITION;
uniform mat4 LIGHT_VIEW_MATRIX;
uniform mat4 LIGHT_PROJECTION_MATRIX;

uniform samplerCube ENVIRONMENT_CUBE_MAP;
uniform sampler2D SHADOW_MAP;

// Constants
const float PI = 3.14159265359;

vec3 Lo(
        vec3 N, vec3 L, vec3 V, vec3 Li,
        vec3 albedo, float roughness, vec3 reflectance, float metallic
);
vec3 Li_PointLight(vec3 I, float L_r);
vec3 Li_DirectionalLight(vec3 E);
float GGX_Trowbridge_Reitz(float roughness, float N_dot_H);
float Schlick_GGZ(float roughness, float N_dot_V, float N_dot_L);
vec3 fresnel(vec3 F0, float V_dot_H);
float getShadowAttenuation(vec4 position);
float getSSAOAttenuation(vec4 position);
vec3 getShadowMapPos(vec4 position);

void main() {
    // Material
    vec3 albedo = vec3(1.0f, 1.0f, 1.0f);
    float roughness = 0.8;
     vec3 reflectance = vec3(0.04);
//     vec3 reflectance = vec3(1.000, 0.766, 0.336); // Gold
    float metallic = 0.0;

    // ...
    vec3 V = normalize(VIEW_POSITION - position.xyz);
    vec3 N = normalize(normal.xyz);

    color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    if (DIRECTIONAL_LIGHT.enabled == 1u) {
        vec3 L = normalize(DIRECTIONAL_LIGHT.direction);
        vec3 E = DIRECTIONAL_LIGHT.irradiance;
        vec3 Li = Li_DirectionalLight(E);

        color.xyz += Lo(N, L, V, Li, albedo, roughness, reflectance, metallic);

        if (DIRECTIONAL_LIGHT.enableShadows != 0u) {
            color.xyz *= getShadowAttenuation(position);
        }
        if (DIRECTIONAL_LIGHT.enableSSAO != 0u) {
            color.xyz *= getSSAOAttenuation(position);
        }
    }

    if (POINT_LIGHT.enabled == 1u) {
        vec3 I = POINT_LIGHT.radiantIntensity;
        vec3 L_vec = POINT_LIGHT.translation - position.xyz;
        vec3 L = normalize(L_vec);
        float L_r = length(L_vec);
        vec3 Li = Li_PointLight(I, L_r);

        color.xyz += Lo(N, L, V, Li, albedo, roughness, reflectance, metallic);

        if (POINT_LIGHT.enableShadows != 0u) {
            color.xyz *= getShadowAttenuation(position);
        }
        if (POINT_LIGHT.enableSSAO != 0u) {
            color.xyz *= getSSAOAttenuation(position);
        }
    }

    // Ambient light
    vec3 I = texture(ENVIRONMENT_CUBE_MAP, N).xyz * 500.0f;
    vec3 L = N;
    vec3 Li = I;
    color.xyz += I; // Lo(N, L, V, Li, albedo, roughness, reflectance, metallic);

    // Reflection
//    vec3 R = reflect(-V, N);
//    vec3 L = R;
//    vec3 Li = texture(ENVIRONMENT_CUBE_MAP, R).xyz;
//    color.xyz += Lo(N, L, V, Li, albedo, roughness, reflectance, metallic);
}

vec3 Lo(
        vec3 N, vec3 L, vec3 V, vec3 Li,
        vec3 albedo, float roughness, vec3 reflectance, float metallic
) {
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

vec3 Li_DirectionalLight(vec3 E) {
    return E;
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

float getShadowAttenuation(vec4 position) {
    float attenuation = 0.0f;
    float itCount = 0.0f;
    for (float xo = -0.05; xo <= 0.05; xo += 0.01) {
        for (float yo = -0.05; yo <= 0.05; yo += 0.01) {
            for (float zo = -0.05; zo <= 0.05; zo += 0.01) {
                vec3 depthPos = getShadowMapPos(position + vec4(xo, yo, zo, 0.0));
                float depth = texture(SHADOW_MAP, depthPos.xy).r;
                attenuation += depthPos.z <= depth + 1e-4 ? 1.0 : 0.0;
                itCount++;
            }
        }
    }
    return attenuation / itCount;
}

float getSSAOAttenuation(vec4 position) {
    float attenuation = 0.0f;
    float itCount = 0.0f;
    for (float xo = -0.05; xo <= 0.05; xo += 0.01) {
        for (float yo = -0.05; yo <= 0.05; yo += 0.01) {
            for (float zo = -0.05; zo <= 0.05; zo += 0.01) {
                vec3 depthPos = getShadowMapPos(position + vec4(xo, yo, zo, 0.0));
                float depth = texture(SHADOW_MAP, depthPos.xy).r;
                attenuation += depthPos.z <= depth + 1e-4 ? 1.0 : 0.0;
                itCount++;
            }
        }
    }
    attenuation /= itCount;
    return clamp(pow(attenuation * 2, 2.0), 0.0, 1.0);
}

vec3 getShadowMapPos(vec4 position) {
    vec4 depthPos = LIGHT_PROJECTION_MATRIX * LIGHT_VIEW_MATRIX * position;
    depthPos /= depthPos.w;
    depthPos = depthPos * 0.5 + 0.5;
    return depthPos.xyz;
}