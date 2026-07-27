#include "CubeMap.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <PineEngine/util/ResourceManager/ResourceManager.h>
#include <PineEngine/rendering/Shading/GraphicShader/GraphicShader.h>

namespace PineEngine {
    CubeMap::CubeMap(const Path &path, const RendererBackend &backend)
        : Resource(path),
          backend(backend),
          cubeMapSourceTexture(ResourceManager::load<Texture>(path, backend))
          // cubeMapFrameBuffer(ResourceManager::load<FrameBuffer>(Path::inMemory(), backend, FrameBufferOptions{.cubeMap = true})),
          // cubeMapGeometry(ResourceManager::load<GeometryBuffer>(GeometryBuffer::QUAD, backend))
    {
        LOG_CONSTRUCTOR(FORMAT("CubeMap[{}]", this->getPath().asString()));
    }

    CubeMap::~CubeMap() {
        LOG_DESTRUCTOR(FORMAT("CubeMap[{}]", this->getPath().asString()));
    }

    void CubeMap::performLoad() {
        // glm::mat4 captureProjection = glm::perspective(
        //     glm::radians(90.0f),
        //     1.0f,
        //     0.1f,
        //     10.0f
        // );
        //
        // const glm::mat4 views[6] = {
        //     glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)),
        //     glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)),
        //     glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
        //     glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)),
        //     glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)),
        //     glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0))
        // };

        // auto cubeMapShader = ResourceManager::load<GraphicShader>(
        //     Path::inMemory(),
        //     Path::inDisk("shaders/PBR_00_CubeMap/vertex.glsl"),
        //     Path::inDisk("shaders/PBR_00_CubeMap/fragment.glsl"),
        //     this->backend
        // );
        // cubeMapShader->prepareForRendering();
        // cubeMapShader->setUniform("PROJECTION_MATRIX", captureProjection);
        // cubeMapShader->setUniformTexture("HDRI", this->cubeMapSourceTexture->getTextureId());
        //
        // this->cubeMapFrameBuffer->resize(512, 512);
        //
        // for (uint32_t i = 0; i < 6; i++) {
        //     this->cubeMapFrameBuffer->attachTextures(i);
        //     this->cubeMapFrameBuffer->prepareForRendering();
        //     this->cubeMapFrameBuffer->clear();
        //     cubeMapShader->setUniform("VIEW_MATRIX", views[i]);
        //
        //     this->cubeMapGeometry->performRendering();
        // }
    }

    void CubeMap::performUnload() {
    }
}
