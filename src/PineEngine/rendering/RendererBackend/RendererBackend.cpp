#include "RendererBackend.h"

#include <PineEngine/util/Time/Time.h>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <string>

namespace PineEngine {
    RendererBackend::RendererBackend(Platform &platform) : platform(platform) {
        this->_initializeOpenGLContext();

        LOG_CONSTRUCTOR("RendererBackend");
    }

    RendererBackend::~RendererBackend() {
        LOG_DESTRUCTOR("RendererBackend");
    }

    void RendererBackend::clearFrame() {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RendererBackend::swapBuffers() {
        this->platform.swapBuffers();
    }

    uint32_t RendererBackend::createGeometry() {
        uint32_t id;
        glGenVertexArrays(1, &id);
        this->_debugMethod(FORMAT("Created geometry {}", id));
        return id;
    }

    void RendererBackend::prepareGeometryForRendering(const uint32_t id) {
        glBindVertexArray(id);
    }

    void RendererBackend::deleteGeometry(const uint32_t id) {
        glDeleteVertexArrays(1, &id);
        this->_debugMethod(FORMAT("Deleted geometry {}", id));
    }

    uint32_t RendererBackend::createDataBuffer() {
        uint32_t id;
        glGenBuffers(1, &id);
        this->_debugMethod(FORMAT("Created data buffer {}", id));
        return id;
    }

    void RendererBackend::allocateDataBuffer(uint32_t id, uint32_t sizeInBytes) {
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, sizeInBytes, nullptr, GL_STATIC_DRAW);

        this->_debugMethod(FORMAT("Allocated data buffer {} with {} bytes", id, sizeInBytes));
    }

    void RendererBackend::populateDataBuffer(const uint32_t id,
                                             const void *data,
                                             const uint32_t offsetInBytes,
                                             const uint32_t sizeInBytes) {
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferSubData(GL_ARRAY_BUFFER, offsetInBytes, sizeInBytes, data);

        this->_debugMethod(
            FORMAT("Populated data buffer {} with offset={} and size={}", id, offsetInBytes, sizeInBytes));
    }

    void RendererBackend::allocateIndexDataBuffer(const uint32_t id, uint32_t sizeInBytes) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeInBytes, nullptr, GL_STATIC_DRAW);

        this->_debugMethod(FORMAT("Allocated index data buffer {} with {} bytes", id, sizeInBytes));
    }

    void RendererBackend::populateIndexDataBuffer(uint32_t id,
                                                  const void *data,
                                                  uint32_t offsetInBytes,
                                                  uint32_t sizeInBytes) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offsetInBytes, sizeInBytes, data);

        this->_debugMethod(FORMAT("Populated index data buffer {} with {} bytes", id, sizeInBytes));
    }

    void RendererBackend::deleteDataBuffer(const uint32_t id) {
        glDeleteBuffers(1, &id);

        this->_debugMethod(FORMAT("Deleted data buffer {}", id));
    }

    void RendererBackend::bindDataBufferToGeometry(const uint32_t geometryId,
                                                   const uint32_t dataBufferId,
                                                   const uint32_t attributeIndex,
                                                   const uint32_t dimensionality,
                                                   const uint32_t combinedDimensionality,
                                                   const uint32_t dataOffset) {
        glBindVertexArray(geometryId);
        glBindBuffer(GL_ARRAY_BUFFER, dataBufferId);
        glVertexAttribPointer(attributeIndex,
                              dimensionality,
                              GL_FLOAT,
                              GL_FALSE,
                              combinedDimensionality * sizeof(float),
                              reinterpret_cast<void *>(dataOffset * sizeof(float)));
        glEnableVertexAttribArray(attributeIndex);

        this->_debugMethod(FORMAT("Bound data buffer {} to geometry {}, attr={}, dim={}, cdim={}, offset={}",
                                  dataBufferId,
                                  geometryId,
                                  attributeIndex,
                                  dimensionality,
                                  combinedDimensionality,
                                  dataOffset));
    }

    void RendererBackend::bindDataBufferToShaderAccess(const uint32_t dataBufferId, const uint32_t attributeIndex) {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, attributeIndex, dataBufferId);
    }

    void RendererBackend::bindIndexDataBufferToGeometry(const uint32_t geometryId, const uint32_t dataBufferId) {
        glBindVertexArray(geometryId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dataBufferId);

        this->_debugMethod(FORMAT("Bound data buffer {} to geometry {}", dataBufferId, geometryId));
    }

    uint32_t RendererBackend::createShaders(
        const std::string &vertexShaderCode,
        const std::string &fragmentShaderCode
    ) {
        const uint32_t vertexShaderId = this->_loadShader(vertexShaderCode, GL_VERTEX_SHADER);
        const uint32_t fragmentShaderId = this->_loadShader(fragmentShaderCode, GL_FRAGMENT_SHADER);

        const uint32_t shaderProgramId = glCreateProgram();
        glAttachShader(shaderProgramId, vertexShaderId);
        glAttachShader(shaderProgramId, fragmentShaderId);
        glLinkProgram(shaderProgramId);

        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);

        if (!success) {
            glGetProgramInfoLog(shaderProgramId, 512, nullptr, infoLog);
            throw std::runtime_error(std::string("Failed to link shader program\n") + infoLog);
        }

        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);

        this->_debugMethod(FORMAT("Created shaders {}", shaderProgramId));

        return shaderProgramId;
    }

    uint32_t RendererBackend::createComputeShader(const std::string &computeShaderCode) {
        const uint32_t computeShaderId = this->_loadShader(computeShaderCode, GL_COMPUTE_SHADER);

        const uint32_t shaderProgramId = glCreateProgram();
        glAttachShader(shaderProgramId, computeShaderId);
        glLinkProgram(shaderProgramId);

        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);

        if (!success) {
            glGetProgramInfoLog(shaderProgramId, 512, nullptr, infoLog);
            throw std::runtime_error(std::string("Failed to link shader program\n") + infoLog);
        }

        glDeleteShader(computeShaderId);

        this->_debugMethod(FORMAT("Created shaders {}", shaderProgramId));

        return shaderProgramId;
    }

    void RendererBackend::prepareShadersForRendering(const uint32_t id) {
        glUseProgram(id);
    }

    void RendererBackend::deleteShaders(const uint32_t id) {
        glDeleteProgram(id);

        this->_debugMethod(FORMAT("Deleted shaders {}", id));
    }

    void RendererBackend::setUniform(const uint32_t shaderId, const std::string &name,
                                     const std::vector<uint32_t> &value) {
        const int32_t uniformLocation = this->_getUniformLocation(shaderId, name);
        if (uniformLocation == -1) return;

        glUseProgram(shaderId);

        if (value.size() == 1) {
            glUniform1ui(static_cast<GLint>(uniformLocation), value[0]);
        } else if (value.size() == 3) {
            glUniform3ui(static_cast<GLint>(uniformLocation), value[0], value[1], value[2]);
        } else if (value.size() == 4) {
            glUniform4ui(static_cast<GLint>(uniformLocation), value[0], value[1], value[2], value[3]);
        } else {
            throw std::runtime_error("Uniform size not implemented!");
        }

        this->_debugMethod("Set uniform", true);
    }

    void RendererBackend::setUniform(const uint32_t shaderId, const std::string &name,
                                     const std::vector<float> &value) {
        const int32_t uniformLocation = this->_getUniformLocation(shaderId, name);
        if (uniformLocation == -1) return;

        glUseProgram(shaderId);

        if (value.size() == 1) {
            glUniform1f(static_cast<GLint>(uniformLocation), value[0]);
        } else if (value.size() == 3) {
            glUniform3f(static_cast<GLint>(uniformLocation), value[0], value[1], value[2]);
        } else if (value.size() == 4) {
            glUniform4f(static_cast<GLint>(uniformLocation), value[0], value[1], value[2], value[3]);
        } else {
            throw std::runtime_error("Uniform size not implemented!");
        }

        this->_debugMethod("Set uniform", true);
    }

    void RendererBackend::setUniform(const uint32_t shaderId, const std::string &name, const glm::mat4 &value) {
        const int32_t uniformLocation = this->_getUniformLocation(shaderId, name);
        if (uniformLocation == -1) return;

        glUseProgram(shaderId);
        glUniformMatrix4fv(static_cast<GLint>(uniformLocation), 1, GL_FALSE, glm::value_ptr(value));

        this->_debugMethod("Set uniform", true);
    }

    void RendererBackend::setUniformTexture(
        const uint32_t shaderId,
        const std::string &name,
        const uint32_t textureId
    ) {
        const int32_t uniformLocation = this->_getUniformLocation(shaderId, name);
        if (uniformLocation == -1) return;

        glUseProgram(shaderId);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glUniform1i(uniformLocation, 0);

        this->_debugMethod("Set uniform texture", true);
    }

    void RendererBackend::executeComputeShader(const uint32_t shaderId, const uint32_t &x, const uint32_t &y) {
        glUseProgram(shaderId);

        for (uint32_t xi = 0; xi < x; xi += 0xFFFF) {
            this->setUniform(shaderId, "X_OFFSET", std::vector{static_cast<float>(xi)});
            for (uint32_t yi = 0; yi < y; yi += 0xFFFF) {
                this->setUniform(shaderId, "Y_OFFSET", std::vector{static_cast<float>(yi)});
                glDispatchCompute(std::min(x - xi, 0xFFFFu), std::min(y - yi, 0xFFFFu), 1);
            }
        }

        this->_debugMethod("Execute compute shader", true);
    }

    void RendererBackend::waitComputeShader() {
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }

    uint32_t RendererBackend::createTexture() {
        uint32_t id;
        glGenTextures(1, &id);
        return id;
    }

    void RendererBackend::allocateColorTexture(const uint32_t textureId, const uint32_t width, const uint32_t height) {
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
        this->_debugMethod("Allocate color texture", true);
    }

    void RendererBackend::allocateDepthTexture(const uint32_t textureId, const uint32_t width, const uint32_t height) {
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        this->_debugMethod("Allocate depth texture", true);
    }

    void RendererBackend::bindTextureForCompute(const uint32_t textureId, const uint32_t attributeIndex) {
        glBindTexture(GL_TEXTURE_2D, textureId);
        glBindImageTexture(
            attributeIndex,
            textureId,
            0,
            GL_FALSE,
            0,
            GL_READ_WRITE,
            GL_RGBA32F
        );
        this->_debugMethod("Bind texture for compute", true);
    }

    void RendererBackend::configureTextureFilterNearest(const uint32_t textureId) {
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        this->_debugMethod("Configure texture filters", true);
    }

    void RendererBackend::configureTextureClampToEdge(const uint32_t textureId) {
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        this->_debugMethod("Configure texture wrapping", true);
    }

    void RendererBackend::deleteTexture(const uint32_t textureId) {
        glDeleteTextures(1, &textureId);
    }

    uint32_t RendererBackend::createFrameBuffer() {
        uint32_t id;
        glGenFramebuffers(1, &id);
        return id;
    }

    uint32_t RendererBackend::createDepthFrameBuffer() {
        uint32_t id;
        glGenRenderbuffers(1, &id);
        return id;
    }

    void RendererBackend::allocateDepthFrameBuffer(
        const uint32_t frameBufferId,
        const uint32_t width,
        const uint32_t height
    ) {
        glBindRenderbuffer(GL_RENDERBUFFER, frameBufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        this->_debugMethod("Allocate depth texture", true);
    }

    void RendererBackend::attachColorTextureToFrameBuffer(const uint32_t frameBufferId, const uint32_t textureId) {
        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

        this->_debugMethod("Attach color texture to frame buffer", true);
    }

    void RendererBackend::attachDepthTextureToFrameBuffer(const uint32_t frameBufferId) {
        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frameBufferId);

        this->_debugMethod("Attach depth texture to frame buffer", true);
    }

    void RendererBackend::attachDepthTextureToFrameBuffer(const uint32_t frameBufferId, const uint32_t textureId) {
        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);

        this->_debugMethod("Attach depth texture to frame buffer", true);
    }

    void RendererBackend::prepareFrameBufferForRendering(
        const uint32_t frameBufferId,
        const uint32_t width,
        const uint32_t height
    ) {
        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
        glViewport(0, 0, width, height);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Framebuffer not loaded!");
        }
    }

    void RendererBackend::deleteFrameBuffer(const uint32_t frameBufferId) {
        glDeleteFramebuffers(1, &frameBufferId);
    }

    void RendererBackend::drawTriangles(const uint32_t vertexCount) {
        glDrawElements(GL_TRIANGLES, static_cast<GLint>(vertexCount), GL_UNSIGNED_INT, nullptr);

        this->_debugMethod("Draw", true);
    }

    void RendererBackend::drawWireframe(const uint32_t vertexCount) {
        glDrawElements(GL_LINES, static_cast<GLint>(vertexCount), GL_UNSIGNED_INT, nullptr);

        this->_debugMethod("Draw", true);
    }

    void RendererBackend::_initializeOpenGLContext() {
        if (!gladLoadGLLoader(this->platform.getOpenGLProcAddress())) {
            throw std::runtime_error("Failed to initialize OpenGL context!");
        }

        this->platform.addResizeListener([](const uint32_t width, const uint32_t height) {
            glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
        });

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
    }

    uint32_t RendererBackend::_loadShader(const std::string &shaderCode, const uint32_t shaderType) {
        const uint32_t id = glCreateShader(shaderType);
        const char *code = shaderCode.c_str();
        glShaderSource(id, 1, &code, nullptr);
        glCompileShader(id);

        int success;
        char infoLog[512];
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(id, 512, nullptr, infoLog);
            throw std::runtime_error(std::string("Failed compile shader\n") + infoLog);
        }

        return id;
    }

    int32_t RendererBackend::_getUniformLocation(const uint32_t shaderId, const std::string &name) {
        const int32_t uniformLocation = glGetUniformLocation(shaderId, name.c_str());
        if (uniformLocation == -1) {
            // LOG(FORMAT("WARNING: Uniform \"{}\" not found in the shader!", name));
        }
        return uniformLocation;
    }

    void RendererBackend::_debugMethod(const std::string_view label, const bool errorsOnly) {
#ifndef NDEBUG
        if (!errorsOnly) {
            LOG_METHOD("RendererBackend - " << std::string(label));
        }
        if (const GLenum err = glGetError(); err != GL_NO_ERROR) {
            LOG_METHOD("RendererBackend - OpenGL Error: " << err);
            throw std::runtime_error("Exiting due to OpenGL error!");
        }
#endif
    }
} // namespace PineEngine
