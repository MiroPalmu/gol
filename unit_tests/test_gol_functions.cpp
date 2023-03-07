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

    SECTION("Parsing glider from plaintext") {
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
        // Should be consexpr but g++-11 don't support constexpr vectors
        /* constexpr */ auto glider_cells = gol::parse_cells_from_plaintext<TestType>(glider, {1, 3});

        REQUIRE(gol::testing::ranges_contains_exactly_same_cells<cell_range_t, cell_t, coordinate_t>(
                glider_cells, cell_range_t{{ 2, 3 }, {3, 4}, {1, 5}, {2, 5}, {3, 5}}
        ));

        SECTION("Set glider cells alive from range") {
            gol::set_cells_alive_from_range(glider_cells, game);

            REQUIRE(gol::testing::ranges_contains_exactly_same_cells<cell_range_t, cell_t, coordinate_t>(
                    game.get_alive_cells(), cell_range_t{{ 2, 3 }, {3, 4}, {1, 5}, {2, 5}, {3, 5}}
        ));

        }

    }

    SECTION("Parsing glider from plaintext with junk in the middle") {
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
        // Should be consexpr but g++-11 don't support constexpr vectors
        /* constexpr */ auto glider_cells = gol::parse_cells_from_plaintext<TestType>(glider, {1, 3});

        REQUIRE(gol::testing::ranges_contains_exactly_same_cells<cell_range_t, cell_t, coordinate_t>(
                glider_cells, cell_range_t{{ 2, 3 }, {3, 4}, {1, 5}, {2, 5}, {3, 5}}
        ));
    }

    SECTION("Exentd") {
        game.set_alive(cell_range_t{{-10, 23}, {10, 15}, {1, 2}, {-2, 3}});

        SECTION("Get extent from game") {
            const auto extent = gol::get_extent(game);

            REQUIRE(extent.min_x == -10);
            REQUIRE(extent.max_x == 10);
            REQUIRE(extent.min_y == 2);
            REQUIRE(extent.max_y == 23);
        }
        SECTION("Get extent from alive cells") {
            const auto alive_cells = game.get_alive_cells();
            const auto extent = gol::get_extent<TestType>(alive_cells);

            REQUIRE(extent.min_x == -10);
            REQUIRE(extent.max_x == 10);
            REQUIRE(extent.min_y == 2);
            REQUIRE(extent.max_y == 23);
        }
    }
}