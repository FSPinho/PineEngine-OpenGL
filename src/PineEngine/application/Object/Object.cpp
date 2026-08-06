#include "./Object.h"

namespace PineEngine {
    Object::Object() : id(SerialID::generate()) {
        LOG_CONSTRUCTOR(FORMAT("Object[{}]", this->id));
    }

    Object::~Object() {
        LOG_DESTRUCTOR(FORMAT("Object[{}]", this->id));
    }

    Transform &Object::getTransform() {
        return this->transform;
    }

    std::optional<Material> &Object::getMaterial() {
        return this->material;
    }

    std::optional<PointLight> &Object::getPointLight() {
        return this->pointLight;
    }

    std::optional<DirectionalLight> &Object::getDirectionalLight() {
        return this->directionalLight;
    }

    ResourceHandler<GeometryBuffer> &Object::getGeometry() {
        return this->geometry;
    }

    ResourceHandler<GraphicShader> &Object::getGraphicShader() {
        return this->graphicShader;
    }

    ResourceHandler<ComputeShader> &Object::getComputeShader() {
        return this->computeShader;
    }

    void Object::setTransform(const Transform &transform_) {
        this->transform = transform_;
    }

    void Object::setMaterial(const Material &material_) {
        this->material = material_;
    }

    void Object::setPointLight(const PointLight &pointLight_) {
        this->pointLight = pointLight_;
    }

    void Object::setDirectionalLight(const DirectionalLight &directionalLight_) {
        this->directionalLight = directionalLight_;
    }

    void Object::setGeometry(const ResourceHandler<GeometryBuffer> &geometry_) {
        this->geometry = geometry_;
    }

    void Object::setGraphicShader(const ResourceHandler<GraphicShader> &graphicShader_) {
        this->graphicShader = graphicShader_;
    }

    void Object::setComputeShader(const ResourceHandler<ComputeShader> &computeShader_) {
        this->computeShader = computeShader_;
    }

    bool Object::operator==(const Object &other) const {
        return this->id == other.id;
    }
} // namespace PineEngine
