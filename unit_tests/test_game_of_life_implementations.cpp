
#include <catch2/catch_template_test_macros.hpp>
#include <gol_concepts.hpp>
#include <gol_functions.hpp>
#include <MapGOL.hpp>


TEMPLATE_PRODUCT_TEST_CASE("Test implementations of GOL", "[implementation]", (gol::MapGOL), (int32_t, int64_t, uint32_t, uint64_t)) {
    gol::game_of_life auto game = TestType{ };

    SECTION("Cells are set alive and killed on command") {
        REQUIRE(!game.is_alive({0, 0}));

        game.set_alive({0, 0});

        REQUIRE(game.is_alive({0, 0}));

        game.kill({0, 0});

        REQUIRE(!game.is_alive({0, 0}));
    }

    SECTION("Cells are set alive and killed on command but with cell range") {
        REQUIRE(!game.is_alive({0, 0}));

        game.set_alive({ {0, 0}, {1, 0}, {1, 1} });

        REQUIRE(game.is_alive({0, 0}));
        REQUIRE(game.is_alive({1, 0}));
        REQUIRE(game.is_alive({1, 1}));

        game.kill({ {1, 0}, {1, 1} });

        REQUIRE(!game.is_alive({1, 0}));
        REQUIRE(!game.is_alive({1, 1}));

        REQUIRE(game.is_alive({0, 0}));
        

    }

}