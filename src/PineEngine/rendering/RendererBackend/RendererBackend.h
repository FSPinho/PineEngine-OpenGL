#pragma once

#include <string>
#include <vector>
#include <PineEngine/platform/Platform/Platform.h>


namespace PineEngine {
    class RendererBackend {
    public :
        explicit RendererBackend(Platform &platform);
        ~RendererBackend();
        void clearFrame();
        void swapBuffers();

        uint32_t createGeometry();
        void prepareGeometryForRendering(uint32_t id);
        void deleteGeometry(uint32_t id);

        uint32_t createDataBuffer();
        void populateVecFloat3DataBuffer(uint32_t id, const std::vector<float> &data);
        void populateIndexDataBuffer(uint32_t id, const std::vector<int> &data);
        void deleteDataBuffer(uint32_t id);
        void bindVecFloat3DataBufferToGeometry(
            uint32_t geometryId,
            uint32_t dataBufferId,
            uint32_t attributeIndex
        );
        void bindIndexDataBufferToGeometry(
            uint32_t geometryId,
            uint32_t dataBufferId
        );

        uint32_t createShaders(
            const std::string &vertexShaderCode,
            const std::string &fragmentShaderCode
        );
        void prepareShadersForRendering(uint32_t id);
        void deleteShaders(uint32_t id);

        void drawTriangles(uint32_t vertexCount);

    private:
        Platform &platform;

        void _initializeOpenGLContext();
        void _initializeViewport();

        uint32_t _loadShader(const std::string &shaderCode, uint32_t shaderType);
        void _debugMethod(std::string_view label, bool errorsOnly = false);
    };
}
