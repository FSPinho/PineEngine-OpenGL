#pragma once

#include <PineEngine/platform/Platform/Platform.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace PineEngine {
    class RendererBackend {
    public:
        explicit RendererBackend(Platform &platform);
        ~RendererBackend();

        void clearFrame(uint32_t frameBufferId = 0);
        void clearColor(uint32_t frameBufferId = 0);
        void clearDepth(uint32_t frameBufferId = 0);
        void enableBlendOverrideBySRCAlpha();
        void enableBlendOneOne();
        void disableBlend();
        void swapBuffers();

        uint32_t createGeometry();
        void prepareGeometryForRendering(uint32_t id);
        void deleteGeometry(uint32_t id);

        uint32_t createDataBuffer();
        void allocateDataBuffer(uint32_t id, uint32_t sizeInBytes);
        void populateDataBuffer(uint32_t id, const void *data, uint32_t offsetInBytes, uint32_t sizeInBytes);
        void allocateIndexDataBuffer(uint32_t id, uint32_t sizeInBytes);
        void populateIndexDataBuffer(uint32_t id, const void *data, uint32_t offsetInBytes, uint32_t sizeInBytes);
        void deleteDataBuffer(uint32_t id);
        void bindDataBufferToGeometry(uint32_t geometryId, uint32_t dataBufferId, uint32_t attributeIndex,
                                      uint32_t dimensionality, uint32_t combinedDimensionality, uint32_t dataOffset);
        void bindDataBufferToShaderAccess(uint32_t dataBufferId, uint32_t attributeIndex);
        void bindIndexDataBufferToGeometry(uint32_t geometryId, uint32_t dataBufferId);

        uint32_t createShaders(
            const std::string &vertexShaderCode,
            const std::string &fragmentShaderCode
        );
        uint32_t createComputeShader(
            const std::string &computeShaderCode
        );
        void prepareShadersForRendering(uint32_t id);
        void deleteShaders(uint32_t id);

        void setUniform(uint32_t shaderId, const std::string &name, const std::vector<uint32_t> &value);
        void setUniform(uint32_t shaderId, const std::string &name, const std::vector<float> &value);
        void setUniform(uint32_t shaderId, const std::string &name, const glm::mat4 &value);
        void setUniformTexture(uint32_t shaderId, const std::string &name, uint32_t textureId, bool multisampled = false, uint32_t attributeIndex = 0);
        void setUniformCubeMapTexture(uint32_t shaderId, const std::string &name, uint32_t textureId);

        void executeComputeShader(uint32_t shaderId, const uint32_t &x, const uint32_t &y);
        void waitComputeShader();

        uint32_t createTexture();
        void allocateColorTexture(uint32_t textureId, uint32_t width, uint32_t height, bool multisampled = false);
        void allocateColorTexture(uint32_t textureId, uint32_t width, uint32_t height, const float *data = nullptr, bool cubeMap = false);
        void allocateDepthTexture(uint32_t textureId, uint32_t width, uint32_t height, bool multisampled = false);
        void bindTextureForCompute(uint32_t textureId, uint32_t attributeIndex);
        void configureTextureFilterNearest(uint32_t textureId, bool cubeMap = false);
        void configureTextureFilterLinear(uint32_t textureId, bool cubeMap = false);
        void configureTextureClampToEdge(uint32_t textureId, bool cubeMap = false);
        void deleteTexture(uint32_t textureId);

        uint32_t createFrameBuffer();
        void attachColorTextureToFrameBuffer(uint32_t frameBufferId, uint32_t textureId, uint32_t attachmentIndex, bool multisampled = false);
        void attachCubeMapTextureToFrameBuffer(uint32_t frameBufferId, uint32_t textureId, uint32_t attachmentIndex, uint32_t faceIndex);
        void attachDepthTextureToFrameBuffer(uint32_t frameBufferId, uint32_t textureId, bool multisampled = false);
        void prepareFrameBufferForRendering(uint32_t frameBufferId, uint32_t layers, uint32_t width, uint32_t height);
        void deleteFrameBuffer(uint32_t frameBufferId);

        void drawTriangles(uint32_t vertexCount);
        void drawWireframe(uint32_t vertexCount);

    private:
        Platform &platform;

        void _initializeOpenGLContext();
        uint32_t _loadShader(const std::string &shaderCode, uint32_t shaderType);
        int32_t _getUniformLocation(uint32_t shaderId, const std::string &name);
        void _debugMethod(std::string_view label, bool errorsOnly = false);
    };
} // namespace PineEngine
