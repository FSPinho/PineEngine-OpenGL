#include "Texture.h"

#include <stb_image.h>
#include <tinyexr.h>
#include <stdexcept>

namespace PineEngine {
    Texture::Texture(const Path &path, const RendererBackend &backend) : Resource(path), backend(backend) {
        LOG_CONSTRUCTOR(FORMAT("Texture[{}]", this->getPath().asString()));
    }


    Texture::~Texture() {
        LOG_DESTRUCTOR(FORMAT("Texture[{}]", this->getPath().asString()));
    }

    uint32_t Texture::getTextureId() const {
        return this->textureId;
    }

    void Texture::performLoad() {
        this->textureId = this->backend.createTexture();

        float *rgba;
        int width;
        int height;
        const char *err;

        const int res = LoadEXR(&rgba, &width, &height, this->getPath().asAbsolutePathString().c_str(), &err);
        if (res != TINYEXR_SUCCESS) {
            throw std::runtime_error(FORMAT("Failed to load image: {}", res));
        }

        this->backend.allocateColorTexture(this->textureId, width, height, rgba);
        this->backend.configureTextureFilterLinear(this->textureId);
        this->backend.configureTextureClampToEdge(this->textureId);
        std::free(rgba);

        this->isLoaded = true;
    }

    void Texture::performUnload() {
        if (this->isLoaded) {
            this->backend.deleteTexture(this->textureId);
        }
    }
}
