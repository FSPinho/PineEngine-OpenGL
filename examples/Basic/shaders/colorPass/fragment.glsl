#version 430 core

struct ShadowVolumeCell {
    float value;
};
layout(std430, binding = 0) coherent buffer ShadowVolumeBuffer {
    ShadowVolumeCell shadowVolume[];
};

uniform sampler2D POSITION;
uniform mat4 VIEW_MATRIX;
uniform mat4 PROJECTION_MATRIX;
uniform uint VOLUME_RESOLUTION;

in vec2 uv;
out vec4 fragmentOutColor;

void main() {
    vec4 position = texture(POSITION, uv);

    vec4 projectedPosition = PROJECTION_MATRIX * VIEW_MATRIX * position;
    uint volumeX = uint(clamp(projectedPosition.x * 0.5 + 0.5, 0.0, 1.0) * VOLUME_RESOLUTION - 1);
    uint volumeY = uint(clamp(projectedPosition.y * 0.5 + 0.5, 0.0, 1.0) * VOLUME_RESOLUTION - 1);
    uint volumeZ = uint(clamp(projectedPosition.z * 0.5 + 0.5, 0.0, 1.0) * VOLUME_RESOLUTION - 1);

    ShadowVolumeCell shadowVolumeCell = shadowVolume[
        volumeX * VOLUME_RESOLUTION * VOLUME_RESOLUTION +
        volumeY * VOLUME_RESOLUTION +
        volumeZ
    ];

    fragmentOutColor = position * shadowVolumeCell.value;
}
