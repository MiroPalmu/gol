#include <catch2/catch_template_test_macros.hpp>
#include <gol_testing.hpp>
#include <gol_utility.hpp>
#include <gol_concepts.hpp>

#include <vector>

TEMPLATE_TEST_CASE("Tests testing functions", "[testing]", int32_t, int64_t) {
    

    SECTION("Testing functions related to cell ranges") {
        using coordinate_t = TestType;
        using cell_t = gol::NaturalCell<TestType>;
        using cell_range_t = std::vector<cell_t>;

        gol::game_of_life_cell_range<cell_t, coordinate_t> auto cell_range = cell_range_t{ {-1, 1}, {1, 1}, {-1, -1}, {1, -1} };

        SECTION("Two empty cell ranges are the same") {
            const auto empty_1 = cell_range_t{ };
            const auto empty_2 = cell_range_t{ };
            REQUIRE(gol::testing::ranges_contains_exactly_same_cells<cell_range_t, cell_t, coordinate_t>(empty_1, empty_2));
        }

        SECTION("Two cell ranges are the same (even if in different order)") {
            gol::game_of_life_cell_range<cell_t, coordinate_t> auto another_cell_range = cell_range_t{ {-1, 1}, {-1, -1}, {1, 1}, {1, -1} };

            REQUIRE(gol::testing::ranges_contains_exactly_same_cells<cell_range_t, cell_t, coordinate_t>(cell_range, another_cell_range));
            REQUIRE(gol::testing::ranges_contains_exactly_same_cells<cell_range_t, cell_t, coordinate_t>(another_cell_range, cell_range));
        }
        
 
        SECTION("Two cell ranges differ by amount of cells") {
            gol::game_of_life_cell_range<cell_t, coordinate_t> auto another_cell_range = cell_range_t{ {-1, 1}, {1, 1}, {-1, -1} };

            REQUIRE(!gol::testing::ranges_contains_exactly_same_cells<cell_range_t, cell_t, coordinate_t>(cell_range, another_cell_range));
            REQUIRE(!gol::testing::ranges_contains_exactly_same_cells<cell_range_t, cell_t, coordinate_t>(another_cell_range, cell_range));
        }

        SECTION("Two cell ranges differ by one cell repeating") {
            gol::game_of_life_cell_range<cell_t, coordinate_t> auto another_cell_range = cell_range_t{ {-1, 1}, {1, 1}, {-1, -1}, {-1, -1} };

            REQUIRE(!gol::testing::ranges_contains_exactly_same_cells<cell_range_t, cell_t, coordinate_t>(another_cell_range, cell_range));
            REQUIRE(!gol::testing::ranges_contains_exactly_same_cells<cell_range_t, cell_t, coordinate_t>(cell_range, another_cell_range));
        } 


        SECTION("Two cell ranges differ by not having the same cells") {
            gol::game_of_life_cell_range<cell_t, coordinate_t> auto another_cell_range = cell_range_t{ {-1, 2}, {1, 4}, {-1, -3}, {-2, -1} };

            REQUIRE(!gol::testing::ranges_contains_exactly_same_cells<cell_range_t, cell_t, coordinate_t>(cell_range, another_cell_range));
            REQUIRE(!gol::testing::ranges_contains_exactly_same_cells<cell_range_t, cell_t, coordinate_t>(another_cell_range, cell_range));
        }
 
    }
}