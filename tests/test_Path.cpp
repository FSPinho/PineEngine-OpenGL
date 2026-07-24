#include <PineEngine/util/Path/Path.h>
#include <catch2/catch_test_macros.hpp>

TEST_CASE (
"Path"
)
 {
    SECTION("Created correctly") {
        REQUIRE(PineEngine::Path("").asString() == "");
        REQUIRE(PineEngine::Path("abc").asString() == "abc");
        REQUIRE(PineEngine::Path("abc/").asString() == "abc");
        REQUIRE(PineEngine::Path("abc//").asString() == "abc");
        REQUIRE(PineEngine::Path("/abc").asString() == "abc");
        REQUIRE(PineEngine::Path("//abc").asString() == "abc");
        REQUIRE(PineEngine::Path("/abc/").asString() == "abc");
        REQUIRE(PineEngine::Path("//abc//").asString() == "abc");
        REQUIRE(PineEngine::Path("abc/def").asString() == "abc/def");
        REQUIRE(PineEngine::Path("abc//def").asString() == "abc/def");
        REQUIRE(PineEngine::Path("/abc/def/").asString() == "abc/def");
        REQUIRE(PineEngine::Path("/abc//def/").asString() == "abc/def");
        REQUIRE(PineEngine::Path("//abc//def//").asString() == "abc/def");
    }

    SECTION("Prefix caught correctly") {
        REQUIRE_THROWS(PineEngine::Path("abc:"));
        REQUIRE_THROWS(PineEngine::Path("abc:a"));
        REQUIRE_THROWS(PineEngine::Path("ab/:/"));
        REQUIRE_THROWS(PineEngine::Path("::"));
        REQUIRE_THROWS(PineEngine::Path(":abc:"));
        REQUIRE_THROWS(PineEngine::Path(":abc:def"));
        REQUIRE_THROWS(PineEngine::Path("abc:def:ghi"));

        REQUIRE(PineEngine::Path("").getPrefix() == "");
        REQUIRE(PineEngine::Path("src").getPrefix() == "");
        REQUIRE(PineEngine::Path("src:/").getPrefix() == "src:/");
        REQUIRE(PineEngine::Path("src://").getPrefix() == "src:/");
        REQUIRE(PineEngine::Path("src://abc").getPrefix() == "src:/");

        REQUIRE(PineEngine::Path("src://abc").asString() == "src://abc");
        REQUIRE(PineEngine::Path("src://abc/").asString() == "src://abc");
        REQUIRE(PineEngine::Path("src://abc//").asString() == "src://abc");
        REQUIRE(PineEngine::Path("src:///abc").asString() == "src://abc");
        REQUIRE(PineEngine::Path("src:///abc/").asString() == "src://abc");
        REQUIRE(PineEngine::Path("src:///abc///").asString() == "src://abc");
        REQUIRE(PineEngine::Path("src://abc/def").asString() == "src://abc/def");
        REQUIRE(PineEngine::Path("src://abc//def").asString() == "src://abc/def");
    }
}
