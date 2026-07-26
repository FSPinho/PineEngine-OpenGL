#version 430

layout(local_size_x = 1, local_size_y = 1) in;

struct ShadowVolumeCell {
    float value;
};
layout(std430, binding = 0) coherent buffer ShadowVolumeBuffer {
    ShadowVolumeCell shadowVolume[];
};

uniform sampler2D POSITION;
uniform uint VOLUME_RESOLUTION;

void main() {
    uint x = gl_GlobalInvocationID.x;

    for (uint xi = 0; xi < VOLUME_RESOLUTION; xi++) {
        for (uint yi = 0; yi < VOLUME_RESOLUTION; yi++) {
            for (uint zi = 0; zi < VOLUME_RESOLUTION; zi++) {
                uint i = xi * VOLUME_RESOLUTION * VOLUME_RESOLUTION + yi * VOLUME_RESOLUTION + zi;
                shadowVolume[i].value = yi < VOLUME_RESOLUTION / 2 ? 0.0 : 1.0;
            }
        }
    }
}