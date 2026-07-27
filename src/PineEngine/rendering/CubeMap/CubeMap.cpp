#include "CubeMap.h"

namespace PineEngine {
    CubeMap::CubeMap(const Path &path, const RendererBackend &backend) : Resource(path), backend(backend) {
        LOG_CONSTRUCTOR(FORMAT("CubeMap[{}]", this->getPath().asString()));
    }

    CubeMap::~CubeMap() {
        LOG_DESTRUCTOR(FORMAT("CubeMap[{}]", this->getPath().asString()));
    }

    void CubeMap::performLoad() {
        this->textureId = this->backend.createTexture();
        this->isLoaded = true;
    }

    void CubeMap::performUnload() {
        if (this->isLoaded) {
            this->backend.deleteTexture(this->textureId);
        }
    }
}
