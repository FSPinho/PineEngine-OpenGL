#pragma once

#include <PineEngine/rendering/RendererComponent/RendererComponent.h>
#include <PineEngine/util/GeometryLoader/GeometryLoader.h>
#include <PineEngine/util/Path/Path.h>
#include <PineEngine/util/Resource/Resource.h>
#include <vector>

namespace PineEngine {

class GeometryBuffer : public Resource, public RendererComponent {
  public:
    explicit GeometryBuffer(const Path &path, const GeometryPreset &preset, RendererBackend &backend);
    ~GeometryBuffer() override;

    void performRendering() override;

  protected:
    void performLoad() override;
    void performUnload() override;

  private:
    GeometryLoader loader;

    uint32_t geometryId = 0;
    uint32_t verticesBufferId = 0;
    uint32_t indicesBufferId = 0;
    bool isGeometryLoaded = false;
    uint32_t indicesCount = 0;

    void _validateGeometry(const std::vector<VertexData> &verticesData, const std::vector<uint32_t> &indices);
    void _loadGeometry(const std::vector<VertexData> &verticesData, const std::vector<uint32_t> &indices);
    void _unloadGeometry();
};
} // namespace PineEngine
