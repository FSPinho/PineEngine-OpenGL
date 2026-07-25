#include <PineEngine/util/OcclusionMap/OcclusionMap.h>
#include <catch2/catch_test_macros.hpp>


TEST_CASE("OcclusionMap") {
    SECTION("Case 1") {
        PineEngine::OcclusionMap map({0.0f, 10.0f, 0.0f}, std::numbers::pi / 180.0);

        const std::vector vertices{
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
        };
        const std::vector<uint32_t> indices {0, 1, 2};
        map.computeFrom(vertices, indices);

        REQUIRE(!map.isOccluded(0, vertices, indices));
    }

    SECTION("Case 2") {
        PineEngine::OcclusionMap map({0.5f, 10.0f, 0.5f}, std::numbers::pi / 4.0);

        const std::vector vertices{
            -1.0f, 0.0f, -1.0f,
            1.0f, 0.0f, -0.0f,
            0.5f, 0.0f, 1.0f,

            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -0.0f,
            0.5f, 1.0f, 1.0f,

            1.0f, 1.0f, 1.0f,
        };
        const std::vector<uint32_t> indices {
            0, 1, 2,
            3, 4, 5,
            3, 4, 6
        };
        map.computeFrom(vertices, indices);

        REQUIRE(map.isOccluded(0, vertices, indices));
        REQUIRE(map.isOccluded(1, vertices, indices));
        REQUIRE(map.isOccluded(2, vertices, indices));
        REQUIRE(!map.isOccluded(3, vertices, indices));
        REQUIRE(!map.isOccluded(4, vertices, indices));
        REQUIRE(!map.isOccluded(5, vertices, indices));
        REQUIRE(!map.isOccluded(6, vertices, indices));
    }
}
