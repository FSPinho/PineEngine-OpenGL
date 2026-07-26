#version 430 core

struct ShadowVolumeCell {
    float value;
};
layout(std430, binding = 0) coherent buffer ShadowVolumeBuffer {
    ShadowVolumeCell shadowVolume[];
};

uniform sampler2D COLOR;
uniform mat4 VIEW_MATRIX;
uniform mat4 PROJECTION_MATRIX;
uniform uint VOLUME_RESOLUTION;

in vec2 uv;
out vec4 fragmentOutColor;

void main() {
    fragmentOutColor = texture(COLOR, uv);
}
