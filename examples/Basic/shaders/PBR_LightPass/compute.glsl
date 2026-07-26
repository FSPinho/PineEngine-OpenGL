#version 430

layout(local_size_x = 1, local_size_y = 1) in;

struct ShadowVolumeCell {
    float value;
};
layout(std430, binding = 0) coherent buffer ShadowVolumeBuffer {
    ShadowVolumeCell shadowVolume[];
};

uniform sampler2D DEPTH;
uniform uint VOLUME_RESOLUTION;
uniform mat4 VIEW_MATRIX;
uniform mat4 PROJECTION_MATRIX;

void main() {
    // uint x = gl_GlobalInvocationID.x;

    for (uint xi = 0; xi < VOLUME_RESOLUTION; xi++) {
        for (uint yi = 0; yi < VOLUME_RESOLUTION; yi++) {
            for (uint zi = 0; zi < VOLUME_RESOLUTION; zi++) {
                uint i = xi * VOLUME_RESOLUTION * VOLUME_RESOLUTION + yi * VOLUME_RESOLUTION + zi;

                float x = (float(xi) / float(VOLUME_RESOLUTION)) * 2.0f - 1.0f;
                float y = (float(yi) / float(VOLUME_RESOLUTION)) * 2.0f - 1.0f;
                float z = (float(zi) / float(VOLUME_RESOLUTION)) * 2.0f - 1.0f;

                vec4 projectedXYZ = PROJECTION_MATRIX * VIEW_MATRIX * vec4(x, y, z, 1.0f);
                projectedXYZ = projectedXYZ * 0.5 + 0.5;

                float depth = texture(DEPTH, projectedXYZ.xy).r;

                shadowVolume[i].value = depth > projectedXYZ.z ? 1.0 : 0.0;
            }
        }
    }
}