#include <PineEngine/util/Resource/Resource.h>
#include <PineEngine/util/ResourceHandler/ResourceHandler.h>
#include <PineEngine/util/ResourceManager/ResourceManager.h>
#include <catch2/catch_test_macros.hpp>
#include <type_traits>

class MyResource : public PineEngine::Resource {
    explicit MyResource(const std::string &path) : Resource(path) {
        this->constructorCallCount++;
    }
    ~MyResource() override {
        this->destructorCallCount++;
    }

    // For testing only

  public:
    int constructorCallCount = 0;
    int destructorCallCount = 0;
    int loadCallCount = 0;
    int unloadCallCount = 0;

  private:
    void load() override { this->loadCallCount++; };
    void unload() override { this->unloadCallCount++; };
};

TEST_CASE("Resource") {
    SECTION("Static checks") {
        STATIC_REQUIRE(std::is_abstract_v<PineEngine::Resource>);
        STATIC_REQUIRE_FALSE(std::is_constructible_v<PineEngine::Resource, MyResource>);
        STATIC_REQUIRE(std::is_base_of_v<PineEngine::Resource, MyResource>);
        STATIC_REQUIRE_FALSE(std::is_abstract_v<MyResource>);
        STATIC_REQUIRE_FALSE(std::is_constructible_v<MyResource>);
    }

    SECTION("Resource acquisition") {
        PineEngine::ResourceManager::load<MyResource>("/some/resource.png");
    }
}
