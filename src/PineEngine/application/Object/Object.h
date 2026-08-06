#pragma once

#include <PineEngine/util/Transform/Transform.h>
#include <PineEngine/util/SerialID/SerialID.h>
#include <PineEngine/util/ResourceHandler/ResourceHandler.h>
#include <PineEngine/rendering/Material/Material.h>
#include <PineEngine/rendering/Light/Light.h>
#include <PineEngine/rendering/GeometryBuffer/GeometryBuffer.h>
#include <PineEngine/rendering/Shading/GraphicShader/GraphicShader.h>
#include <PineEngine/rendering/Shading/ComputeShader/ComputeShader.h>


namespace PineEngine {
    class Object {
    public:
        explicit Object();
        ~Object();

        Transform &getTransform();
        std::optional<Material> &getMaterial();
        std::optional<PointLight> &getPointLight();
        std::optional<DirectionalLight> &getDirectionalLight();
        ResourceHandler<GeometryBuffer> &getGeometry();
        ResourceHandler<GraphicShader> &getGraphicShader();
        ResourceHandler<ComputeShader> &getComputeShader();

        void setTransform(const Transform &transform_);
        void setMaterial(const Material &material_);
        void setPointLight(const PointLight &pointLight_);
        void setDirectionalLight(const DirectionalLight &directionalLight_);
        void setGeometry(const ResourceHandler<GeometryBuffer> &geometry_);
        void setGraphicShader(const ResourceHandler<GraphicShader> &graphicShader_);
        void setComputeShader(const ResourceHandler<ComputeShader> &computeShader_);

        bool operator==(const Object &other) const;

    private:
        ID id;

        Transform transform;
        std::optional<Material> material;
        std::optional<PointLight> pointLight;
        std::optional<DirectionalLight> directionalLight;
        ResourceHandler<GeometryBuffer> geometry;
        ResourceHandler<GraphicShader> graphicShader;
        ResourceHandler<ComputeShader> computeShader;
    };
} // namespace PineEngine
