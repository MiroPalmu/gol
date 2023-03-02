#include <catch2/catch_template_test_macros.hpp>
#include <MapGOL.hpp>
#include <gol_functions.hpp>
#include <gol_testing.hpp>
#include <string_view>

TEMPLATE_PRODUCT_TEST_CASE("Test utility functions", "[utility]", (gol::MapGOL), (int32_t, int64_t)) {
    gol::game_of_life auto game = TestType{ };
    using coordinate_t = typename TestType::coordinate_t;
    using cell_t = typename TestType::cell_t;
    using cell_range_t = typename TestType::cell_range_t;

    SECTION("Adding glider from plaintext") {
        constexpr auto glider = std::string_view {
R"(!Name: Glider
!Author: Richard K. Guy
!The smallest, most common, and first discovered spaceship.
!www.conwaylife.com/wiki/index.php?title=Glider
.O
..O
OOO
)"
        };
        gol::add_cells_from_plaintext_to_game(glider, game, cell_t{ 0, 0 });

        REQUIRE(gol::testing::ranges_contains_exactly_same_cells<cell_range_t, cell_t, coordinate_t>(
                game.get_alive_cells(), cell_range_t{{ 1, 0 }, {2, 1}, {0, 2}, {1, 2}, {2, 2}}
        ));
    }
    SECTION("Adding glider from plaintext with junk in the middle") {
        constexpr auto glider = std::string_view {
R"(!Name: Glider
!Author: Richard K. Guy
fdafdsd
fd
!The smallest, most common, and first discovered spaceship.
!www.conwaylife.com/wiki/index.php?title=Glider
.fdhaO
..fdO
OOfdfdaO
)"
        };
        gol::add_cells_from_plaintext_to_game(glider, game, cell_t{ 0, 0 });

        REQUIRE(gol::testing::ranges_contains_exactly_same_cells<cell_range_t, cell_t, coordinate_t>(
                game.get_alive_cells(), cell_range_t{{ 1, 0 }, {2, 1}, {0, 2}, {1, 2}, {2, 2}}
        ));
    }
}