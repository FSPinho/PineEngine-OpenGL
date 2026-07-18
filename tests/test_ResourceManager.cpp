#include <type_traits>
#include <catch2/catch_test_macros.hpp>
#include <PineEngine/util/Resource/Resource.h>
#include <PineEngine/util/ResourceHandler/ResourceHandler.h>
#include <PineEngine/util/ResourceManager/ResourceManager.h>

class MyResource : public PineEngine::Resource {
    explicit MyResource(const std::string &path) : PineEngine::Resource(path) {
    }

private:
    void load() override;
    void unload() override;
};

TEST_CASE("Resource") {
    SECTION("Static checks") {
        STATIC_REQUIRE(std::is_abstract_v<PineEngine::Resource>);
        STATIC_REQUIRE_FALSE(std::is_constructible_v<PineEngine::Resource, MyResource>);

        STATIC_REQUIRE(std::is_base_of_v<PineEngine::Resource, MyResource>);
        STATIC_REQUIRE_FALSE(std::is_abstract_v<MyResource>);
        STATIC_REQUIRE_FALSE(std::is_constructible_v<MyResource>);
    }
}
