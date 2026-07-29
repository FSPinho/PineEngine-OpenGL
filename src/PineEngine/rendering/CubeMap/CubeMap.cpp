#include "CubeMap.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <PineEngine/util/ResourceManager/ResourceManager.h>
#include <PineEngine/rendering/Shading/GraphicShader/GraphicShader.h>

namespace PineEngine {
    CubeMap::CubeMap(const Path &path, const Path &sourcePath, const RendererBackend &backend)
        : Resource(path),
          backend(backend),
          cubeMapSourceTexture(ResourceManager::load<Texture>(sourcePath, backend)),
          cubeMapFrameBuffer(ResourceManager::load<FrameBuffer>(Path::inMemory(), backend, FrameBufferOptions{.cubeMap = true, .colorLayers = 2})),
          cubeMapGeometry(ResourceManager::load<GeometryBuffer>(GeometryBuffer::CUBE, backend)) {
        LOG_CONSTRUCTOR(FORMAT("CubeMap[{}]", this->getPath().asString()));
    }

    CubeMap::~CubeMap() {
        LOG_DESTRUCTOR(FORMAT("CubeMap[{}]", this->getPath().asString()));
    }

    uint32_t CubeMap::getTextureId(const uint32_t level) {
        return this->cubeMapFrameBuffer->getColorTextureId(level);
    }

    void CubeMap::performLoad() {
        const glm::mat4 captureProjection = glm::perspective(
            glm::radians(90.0f),
            1.0f,
            0.1f,
            10.0f
        );

        const glm::mat4 views[6] = {
            glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)),
            glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)),
            glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
            glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)),
            glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)),
            glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0))
        };

        auto cubeMapShader = ResourceManager::load<GraphicShader>(
            Path::inMemory(),
            Path::inDisk("shaders/PBR_00_CubeMapPrep/vertex.glsl"),
            Path::inDisk("shaders/PBR_00_CubeMapPrep/fragment.glsl"),
            this->backend
        );
        cubeMapShader->prepareForRendering();
        cubeMapShader->setUniform("PROJECTION_MATRIX", captureProjection);
        cubeMapShader->setUniformTexture("HDRI", this->cubeMapSourceTexture->getTextureId());

        this->backend.disableBlend();
        this->cubeMapFrameBuffer->resize(512, 512);

        for (uint32_t i = 0; i < 6; i++) {
            this->cubeMapFrameBuffer->attachTextures(0, i);
            this->cubeMapFrameBuffer->attachTextures(1, i);
            this->cubeMapFrameBuffer->prepareForRendering();
            this->cubeMapFrameBuffer->clear();
            cubeMapShader->setUniform("VIEW_MATRIX", views[i]);

            this->cubeMapGeometry->performRendering();
        }
    }

    void CubeMap::performUnload() {
    }
}
