#include "Application.h"

#include <PineEngine/util/Path/Path.h>


namespace PineEngine {
    Application::Application()
        : rendererBackend(this->platform),
          inputManager(this->platform),
          shadowMapFrameBuffer(ResourceManager::load<FrameBuffer>(
              Path::inMemory(),
              rendererBackend,
              false, true // No multisample, depth
          )),
          colorFrameBuffer(ResourceManager::load<FrameBuffer>(
              Path::inMemory(),
              rendererBackend,
              true, true // Multisample, depth
          )),
          addColorFrameBuffer(ResourceManager::load<FrameBuffer>(
              Path::inMemory(),
              rendererBackend,
              false, false // No multisample, no depth
          )) {
        LOG_CONSTRUCTOR("Application");

        this->platform.addResizeListener([this](const uint32_t width, const uint32_t height) {
            this->camera.setAspect(static_cast<float>(width) / static_cast<float>(height));
        });

        this->addColorQuadObject.setGeometry<GeometryBuffer>(Path::inMemory(), GeometryPreset::QUAD, this->rendererBackend);
        this->addColorQuadObject.setColorShader<GraphicShader>(
            Path::inMemory(),
            Path::inDisk("shaders/PBR_03_AddColor/vertex.glsl"),
            Path::inDisk("shaders/PBR_03_AddColor/fragment.glsl"),
            this->rendererBackend
        );

        this->postProcessingQuadObject.setGeometry<GeometryBuffer>(Path::inMemory(), GeometryPreset::QUAD, this->rendererBackend);
        this->postProcessingQuadObject.setColorShader<GraphicShader>(
            Path::inMemory(),
            Path::inDisk("shaders/PBR_04_PostColor/vertex.glsl"),
            Path::inDisk("shaders/PBR_04_PostColor/fragment.glsl"),
            this->rendererBackend
        );
    }

    Application::~Application() {
        LOG_DESTRUCTOR("Application");
    }

    void Application::mainLoop(const std::function<void(const Tick &)> &onTick) {
        this->platform.mainLoop([this, onTick] {
            const auto tick = this->timer.getNextTick();
            this->inputManager.preProcessMouseEvents(tick);

            const auto [width, height] = this->platform.getSurfaceSize();
            this->shadowMapFrameBuffer->resize(width, height);
            this->colorFrameBuffer->resize(width, height);
            this->addColorFrameBuffer->resize(width, height);

            this->shadowMapFrameBuffer->attachTextures();
            this->colorFrameBuffer->attachTextures();
            this->addColorFrameBuffer->attachTextures();

            bool isFirstPass = true;

            for (const auto &directionalLight: this->rootScene.getDirectionalLights()) {
                // Shadow map
                this->shadowMapFrameBuffer->prepareForRendering();
                this->shadowMapFrameBuffer->clear();
                const auto &target = this->camera.getTarget();
                const auto &translation = target + glm::normalize(directionalLight.direction) * 50.0f;
                Camera lightCamera(translation, target, -5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
                for (auto &object: this->rootScene.getObjects()) {
                    object.performShadowMapPass(tick, lightCamera);
                }

                // Color
                this->rendererBackend.disableBlend();
                this->colorFrameBuffer->prepareForRendering();
                this->colorFrameBuffer->clear();
                for (auto &object: this->rootScene.getObjects()) {
                    object.performColorPass(
                        tick,
                        this->camera,
                        lightCamera,
                        directionalLight,
                        this->shadowMapFrameBuffer->getDepthTextureId()
                    );
                }

                // Add color
                this->rendererBackend.enableBlend();
                this->addColorFrameBuffer->prepareForRendering();
                if (isFirstPass) this->addColorFrameBuffer->clear();
                this->addColorQuadObject.performAddColorPass(this->colorFrameBuffer->getColorTextureId(), true);

                isFirstPass = false;
            }

            // Post color pass
            this->rendererBackend.disableBlend();
            this->rendererBackend.prepareFrameBufferForRendering(0, width, height);
            this->rendererBackend.clearFrame();
            this->postProcessingQuadObject.performPostColorPass(
                this->addColorFrameBuffer->getColorTextureId(),
                this->rootScene.getDirectionalLights(),
                this->rootScene.getPointLights(),
                false
            );

            this->rendererBackend.swapBuffers();

            onTick(tick);

            this->inputManager.postProcessMouseEvents();
        });
    }

    RendererBackend &Application::getRendererBackend() {
        return this->rendererBackend;
    }

    Scene &Application::getRootScene() {
        return this->rootScene;
    }

    Camera &Application::getCamera() {
        return this->camera;
    }

    InputManager &Application::getInputManager() {
        return this->inputManager;
    }
} // namespace PineEngine
