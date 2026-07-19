#include <PineEngine/util/Path/Path.h>
#include <PineEngine/util/Resource/Resource.h>
#include <PineEngine/util/ResourceHandler/ResourceHandler.h>
#include <PineEngine/util/ResourceManager/ResourceManager.h>
#include <catch2/catch_test_macros.hpp>
#include <type_traits>

class TestResource : public PineEngine::Resource {
  public:
    static int constructorCallCount;
    static int destructorCallCount;
    static int loadCallCount;
    static int unloadCallCount;

    explicit TestResource(const PineEngine::Path &path) : Resource(path) { constructorCallCount++; }

    ~TestResource() override { destructorCallCount++; }

  protected:
    void performLoad() override { loadCallCount++; };
    void performUnload() override { unloadCallCount++; };
};

int TestResource::constructorCallCount = 0;
int TestResource::destructorCallCount = 0;
int TestResource::loadCallCount = 0;
int TestResource::unloadCallCount = 0;

TEST_CASE("Resource") {
    SECTION("Static checks") {
        // Resource must be abstract, thus non-constructible
        STATIC_REQUIRE(std::is_abstract_v<PineEngine::Resource>);
        STATIC_REQUIRE_FALSE(std::is_constructible_v<PineEngine::Resource, TestResource>);

        // MyResource must extend from Resource, and be non-abstract
        STATIC_REQUIRE(std::is_base_of_v<PineEngine::Resource, TestResource>);
        STATIC_REQUIRE_FALSE(std::is_abstract_v<TestResource>);
        STATIC_REQUIRE_FALSE(std::is_abstract_v<TestResource>);
        STATIC_REQUIRE(std::is_constructible_v<TestResource, PineEngine::Path>);
    }
}

TEST_CASE("ResourceHandler") {
    // Setup
    TestResource::constructorCallCount = 0;
    TestResource::destructorCallCount = 0;
    TestResource::loadCallCount = 0;
    TestResource::unloadCallCount = 0;

    SECTION("Constructor and destructor run once") {
        {
            auto resource = TestResource(PineEngine::Path("some/resource"));
        }
        REQUIRE(TestResource::constructorCallCount == 1);
        REQUIRE(TestResource::destructorCallCount == 1);
        REQUIRE(TestResource::loadCallCount == 0);
        REQUIRE(TestResource::unloadCallCount == 0);
    }

    SECTION("Resource methods correctly forwarded") {
        {
            auto resource = TestResource(PineEngine::Path("some/resource"));
            auto handler = PineEngine::ResourceHandler(&resource);
            REQUIRE(handler->getPath() == "some/resource");
        }
        REQUIRE(TestResource::constructorCallCount == 1);
        REQUIRE(TestResource::destructorCallCount == 1);
        REQUIRE(TestResource::loadCallCount == 0);
        REQUIRE(TestResource::unloadCallCount == 0);
    }

    SECTION("Move and copy blocked") {
        {
            auto resource = TestResource(PineEngine::Path("some/resource"));
            auto handler = PineEngine::ResourceHandler(&resource);

            STATIC_REQUIRE_FALSE(std::is_copy_constructible_v<PineEngine::ResourceHandler<TestResource>>);
            STATIC_REQUIRE_FALSE(std::is_copy_assignable_v<PineEngine::ResourceHandler<TestResource>>);
        }
        REQUIRE(TestResource::constructorCallCount == 1);
        REQUIRE(TestResource::destructorCallCount == 1);
        REQUIRE(TestResource::loadCallCount == 0);
        REQUIRE(TestResource::unloadCallCount == 0);
    }

    SECTION("Listeners correctly called") {
        int releaseCallCount = 0;
        {
            auto resource = TestResource(PineEngine::Path("some/resource"));
            auto handler = PineEngine::ResourceHandler(&resource);
            handler.addHandlerReleasedListener([&releaseCallCount] { releaseCallCount++; });

            REQUIRE(releaseCallCount == 0);
        }
        REQUIRE(releaseCallCount == 1);
        REQUIRE(TestResource::constructorCallCount == 1);
        REQUIRE(TestResource::destructorCallCount == 1);
        REQUIRE(TestResource::loadCallCount == 0);
        REQUIRE(TestResource::unloadCallCount == 0);
    }
}

TEST_CASE("ResourceManager") {
    // Setup
    TestResource::constructorCallCount = 0;
    TestResource::destructorCallCount = 0;
    TestResource::loadCallCount = 0;
    TestResource::unloadCallCount = 0;

    SECTION("Single acquisition correctly performed") {
        {
            auto handler = PineEngine::ResourceManager::load<TestResource>(PineEngine::Path("some/resource"));
            REQUIRE(PineEngine::ResourceManager::getResourceUsage(PineEngine::Path("some/resource")) == 1);
            REQUIRE(TestResource::constructorCallCount == 1);
            REQUIRE(TestResource::destructorCallCount == 0);
            REQUIRE(TestResource::loadCallCount == 1);
            REQUIRE(TestResource::unloadCallCount == 0);
        }
        REQUIRE(PineEngine::ResourceManager::getResourceUsage(PineEngine::Path("some/resource")) == 0);
        REQUIRE(TestResource::constructorCallCount == 1);
        REQUIRE(TestResource::destructorCallCount == 1);
        REQUIRE(TestResource::loadCallCount == 1);
        REQUIRE(TestResource::unloadCallCount == 1);
    }

    SECTION("Double acquisition correctly performed") {
        {
            auto handler1 = PineEngine::ResourceManager::load<TestResource>(PineEngine::Path("some/resource"));
            REQUIRE(PineEngine::ResourceManager::getResourceUsage(PineEngine::Path("some/resource")) == 1);
            REQUIRE(TestResource::constructorCallCount == 1);
            REQUIRE(TestResource::destructorCallCount == 0);
            REQUIRE(TestResource::loadCallCount == 1);
            REQUIRE(TestResource::unloadCallCount == 0);

            auto handler2 = PineEngine::ResourceManager::load<TestResource>(PineEngine::Path("some/resource"));
            REQUIRE(PineEngine::ResourceManager::getResourceUsage(PineEngine::Path("some/resource")) == 2);
            REQUIRE(TestResource::constructorCallCount == 1);
            REQUIRE(TestResource::destructorCallCount == 0);
            REQUIRE(TestResource::loadCallCount == 1);
            REQUIRE(TestResource::unloadCallCount == 0);
        }
        REQUIRE(PineEngine::ResourceManager::getResourceUsage(PineEngine::Path("some/resource")) == 0);
        REQUIRE(TestResource::constructorCallCount == 1);
        REQUIRE(TestResource::destructorCallCount == 1);
        REQUIRE(TestResource::loadCallCount == 1);
        REQUIRE(TestResource::unloadCallCount == 1);
    }

    SECTION("Multi acquisition correctly performed") {
        {
            auto handler1 = PineEngine::ResourceManager::load<TestResource>(PineEngine::Path("some/resource1"));
            REQUIRE(PineEngine::ResourceManager::getResourceUsage(PineEngine::Path("some/resource1")) == 1);
            REQUIRE(PineEngine::ResourceManager::getResourceUsage(PineEngine::Path("some/resource2")) == 0);
            REQUIRE(TestResource::constructorCallCount == 1);
            REQUIRE(TestResource::destructorCallCount == 0);
            REQUIRE(TestResource::loadCallCount == 1);
            REQUIRE(TestResource::unloadCallCount == 0);

            auto handler2 = PineEngine::ResourceManager::load<TestResource>(PineEngine::Path("some/resource2"));
            REQUIRE(PineEngine::ResourceManager::getResourceUsage(PineEngine::Path("some/resource1")) == 1);
            REQUIRE(PineEngine::ResourceManager::getResourceUsage(PineEngine::Path("some/resource2")) == 1);
            REQUIRE(TestResource::constructorCallCount == 2);
            REQUIRE(TestResource::destructorCallCount == 0);
            REQUIRE(TestResource::loadCallCount == 2);
            REQUIRE(TestResource::unloadCallCount == 0);
        }
        REQUIRE(PineEngine::ResourceManager::getResourceUsage(PineEngine::Path("some/resource1")) == 0);
        REQUIRE(PineEngine::ResourceManager::getResourceUsage(PineEngine::Path("some/resource2")) == 0);
        REQUIRE(TestResource::constructorCallCount == 2);
        REQUIRE(TestResource::destructorCallCount == 2);
        REQUIRE(TestResource::loadCallCount == 2);
        REQUIRE(TestResource::unloadCallCount == 2);
    }
}
