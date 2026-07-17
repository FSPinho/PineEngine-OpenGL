#include "RendererBackend.h"
#include <string>
#include <stdexcept>
#include <cassert>


namespace PineEngine {
    RendererBackend::RendererBackend(Platform &platform) : platform(platform) {
        this->_initializeOpenGLContext();
        this->_initializeViewport();

        LOG_CONSTRUCTOR("RendererBackend");
    }

    RendererBackend::~RendererBackend() {
        LOG_DESTRUCTOR("RendererBackend");
    }

    void RendererBackend::clearFrame() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void RendererBackend::swapBuffers() {
        this->platform.swapBuffers();
    }

    uint32_t RendererBackend::createGeometry() {
        uint32_t id;
        glGenVertexArrays(1, &id);
        this->_debugMethod(FORMAT("RendererBackend - Created geometry {}", id));
        return id;
    }

    void RendererBackend::prepareGeometryForRendering(const uint32_t id) {
        glBindVertexArray(id);
    }

    void RendererBackend::deleteGeometry(const uint32_t id) {
        glDeleteVertexArrays(1, &id);
        this->_debugMethod(FORMAT("RendererBackend - Deleted geometry {}", id));
    }

    uint32_t RendererBackend::createDataBuffer() {
        uint32_t id;
        glGenBuffers(1, &id);
        this->_debugMethod(FORMAT("RendererBackend - Created data buffer {}", id));
        return id;
    }

    void RendererBackend::populateVecFloat3DataBuffer(const uint32_t id, const std::vector<float> &data) {
        assert(data.size() % 3 == 0 && "A vec3 array size should be divisible by 3");

        const auto sizeInBytes = static_cast<GLsizeiptr>(data.size() * sizeof(data[0]));
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeInBytes,
            data.data(),
            GL_STATIC_DRAW
        );

        this->_debugMethod(FORMAT(
            "RendererBackend - Populated vec3 data buffer {} with {} bytes",
            id, sizeInBytes
        ));
    }

    void RendererBackend::populateIndexDataBuffer(uint32_t id, const std::vector<int> &data) {
        const auto sizeInBytes = static_cast<GLsizeiptr>(data.size() * sizeof(data[0]));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeInBytes,
            data.data(),
            GL_STATIC_DRAW
        );

        this->_debugMethod(FORMAT(
            "RendererBackend - Populated int data buffer {} with {} bytes",
            id, sizeInBytes
        ));
    }

    void RendererBackend::deleteDataBuffer(const uint32_t id) {
        glDeleteBuffers(1, &id);

        this->_debugMethod(FORMAT("RendererBackend - Deleted data buffer {}", id));
    }

    void RendererBackend::bindVecFloat3DataBufferToGeometry(
        const uint32_t geometryId,
        const uint32_t dataBufferId,
        const uint32_t attributeIndex
    ) {
        glBindVertexArray(geometryId);
        glBindBuffer(GL_ARRAY_BUFFER, dataBufferId);
        glVertexAttribPointer(
            attributeIndex,
            3,
            GL_FLOAT,
            GL_FALSE,
            3 * sizeof(float),
            nullptr
        );
        glEnableVertexAttribArray(attributeIndex);

        this->_debugMethod(FORMAT(
            "RendererBackend - Bound data buffer {} to geometry {}",
            dataBufferId, geometryId
        ));
    }

    void RendererBackend::bindIndexDataBufferToGeometry(
        const uint32_t geometryId,
        const uint32_t dataBufferId
    ) {
        glBindVertexArray(geometryId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dataBufferId);

        this->_debugMethod(FORMAT(
            "RendererBackend - Bound data buffer {} to geometry {}",
            dataBufferId, geometryId
        ));
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
            throw std::runtime_error(
                std::string("Failed to link shader program\n") + infoLog
            );
        }

        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);

        this->_debugMethod(FORMAT("RendererBackend - Created shaders {}", shaderProgramId));

        return shaderProgramId;
    }

    void RendererBackend::prepareShadersForRendering(const uint32_t id) {
        glUseProgram(id);
    }

    void RendererBackend::deleteShaders(const uint32_t id) {
        glDeleteProgram(id);

        this->_debugMethod(FORMAT("RendererBackend - Deleted shaders {}", id));
    }

    void RendererBackend::drawTriangles(const uint32_t vertexCount) {
        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLint>(vertexCount),
            GL_UNSIGNED_INT,
            nullptr
        );

        this->_debugMethod("Draw", true);
    }

    void RendererBackend::_initializeOpenGLContext() {
        if (!gladLoadGLLoader(this->platform.getOpenGLProcAddress())) {
            throw std::runtime_error("Failed to initialize OpenGL context!");
        }
    }

    void RendererBackend::_initializeViewport() {
        glViewport(0, 0, CONFIG::WINDOW_WIDTH, CONFIG::WINDOW_HEIGHT);
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
            throw std::runtime_error(
                std::string("Failed compile shader\n") + infoLog
            );
        }

        return id;
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
}
