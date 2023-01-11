
#include <catch2/catch_template_test_macros.hpp>
#include <gol_concepts.hpp>
#include <gol_functions.hpp>
#include <gol_testing.hpp>
#include <MapGOL.hpp>

#include <ranges>


TEMPLATE_PRODUCT_TEST_CASE("Test implementations of GOL", "[implementation]", (gol::MapGOL), (int32_t, int64_t)) {
    gol::game_of_life auto game = TestType{ };
    using coordinate_t = typename TestType::coordinate_t;
    using cell_t = typename TestType::cell_t;
    using cell_range_t = typename TestType::cell_range_t;

    SECTION("Cells compare correctly") {
        const auto cell = cell_t{1, 2};

        REQUIRE(cell == cell);
        const auto components_are_correct = (cell.x == 1) && (cell.y == 2);
        REQUIRE(components_are_correct);

        const auto another_cell = cell_t{2, 1};
        REQUIRE(cell != another_cell);
    }

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

    SECTION("Alive cells are given correctly") {
        REQUIRE(std::size(game.get_alive_cells()) == 0);

        const auto cell_01 = cell_t{0, 1};
        const auto cell_11 = cell_t{1, 1};
        const auto cell_00 = cell_t{0, 0};

        game.set_alive({ cell_01, cell_11, cell_00 });

        REQUIRE(std::ranges::size(game.get_alive_cells()) == 3);

        SECTION("Cells set alive are alive") {
            REQUIRE(gol::testing::ranges_contains_exactly_same_cells<cell_range_t, cell_t, coordinate_t>(
                game.get_alive_cells(), cell_range_t{cell_00, cell_01, cell_11}
            ));
        }
        
        SECTION("Cells set alive are alive and killed ones are dead") {
            game.kill({cell_01, cell_11});
        
            REQUIRE(gol::testing::ranges_contains_exactly_same_cells<cell_range_t, cell_t, coordinate_t>(
                game.get_alive_cells(), cell_range_t{cell_00}
            ));
            
        }
    }

}