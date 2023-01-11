#include <gol_concepts.hpp>
#include <concepts>
#include <ranges>
#include <vector>
#include <algorithm>

namespace gol::testing {

    template <typename R, typename C, typename I>
    requires game_of_life_cell_range<R, C, I>
    [[nodiscard]] constexpr auto ranges_contains_exactly_same_cells(const R& range_1, const R& range_2) -> bool {
        if (std::ranges::size(range_1) != std::ranges::size(range_2)) {
            return false; // Different amount of cells
        }

        const auto cells = std::ranges::size(range_1);

        // Because vector of bool is weird we use int as boolean
        auto cells_from_range_2_found_in_range_1 = std::vector<int>(cells, 0);
        auto cells_from_range_1_found_in_range_2 = std::vector<int>(cells, 0);

        for (size_t i = 0; i < cells; ++i) {
            for (size_t j = 0; j < cells; ++j) {
                const auto& cell_1 = range_1[i];
                const auto& cell_2 = range_2[j];

                if (cell_1 == cell_2) {

                    if (cells_from_range_1_found_in_range_2[i] || cells_from_range_2_found_in_range_1[j]) {
                        return false; // Same cell found twice
                    }

                    cells_from_range_1_found_in_range_2[i] = 1;
                    cells_from_range_2_found_in_range_1[j] = 1;
                }

            }
        }
        // To get bool from ints
        auto identiy = [](auto x) noexcept -> bool { return x; };

        return std::ranges::all_of(cells_from_range_1_found_in_range_2, identiy)
                && std::ranges::all_of(cells_from_range_2_found_in_range_1, identiy);

    }

namespace patterns {


    template <typename R, typename C, typename I>
    requires game_of_life_cell_range<R, C, I>
    struct TestPattern {
        R initial_pattern;
        R pattern_after_one_step; // If there no cells effecting evolution of cells in initial_pattern
    };

    /* 
        Rules of Game of Life:
            1) Any live cell with two or three live neighbours survives.
            2) Any dead cell with three live neighbours becomes a live cell.
            3) All other live cells die in the next generation. Similarly, all other dead cells stay dead.

        Following functions tests each rule applied to the center_cell. Other rules may apply to other cells as well.             
     */

    template <typename R, typename C, typename I>
    requires game_of_life_cell_range<R, C, I>
    constexpr auto create_test_pattern_for_first_rule_1(const C& center_cell) -> TestPattern<R, C, I> {
        const auto initial = R{ center_cell, {center_cell.x + 1, center_cell.y + 1}, {center_cell.x + 1, center_cell.y - 1}};
        const auto after_one_step = R{ center_cell, {center_cell.x + 1, center_cell.y} };
        return { initial, after_one_step };
    }

    template <typename R, typename C, typename I>
    requires game_of_life_cell_range<R, C, I>
    constexpr auto create_test_pattern_for_first_rule_2(const C& center_cell) -> TestPattern<R, C, I> {
        const auto initial = R{ center_cell, 
                            {center_cell.x + 1, center_cell.y + 1},
                            {center_cell.x - 1, center_cell.y},
                            {center_cell.x + 1, center_cell.y - 1}
                        };
        const auto after_one_step = R{ center_cell,
                                    {center_cell.x, center_cell.y + 1},    
                                    {center_cell.x, center_cell.y - 1},    
                                    {center_cell.x + 1, center_cell.y}    
                                 };
        return { initial, after_one_step };
    }

    template <typename R, typename C, typename I>
    requires game_of_life_cell_range<R, C, I>
    constexpr auto create_test_pattern_for_second_rule_1(const C& center_cell) -> TestPattern<R, C, I> {
        const auto initial = R{  
                                {center_cell.x + 1, center_cell.y + 1},
                                {center_cell.x - 1, center_cell.y},
                                {center_cell.x + 1, center_cell.y - 1}
                            };
        const auto after_one_step = R{ center_cell };
        return { initial, after_one_step };
    }

    template <typename R, typename C, typename I>
    requires game_of_life_cell_range<R, C, I>
    constexpr auto create_test_pattern_for_third_rule_1(const C& center_cell) -> TestPattern<R, C, I> {
        const auto initial = R{ {center_cell.x + 1, center_cell.y + 1},
                                {center_cell.x + 1, center_cell.y - 1},
                                {center_cell.x - 1, center_cell.y + 1},
                                {center_cell.x - 1, center_cell.y - 1}
                            };
        const auto after_one_step = R{ };
        return { initial, after_one_step };
    }

    template <typename R, typename C, typename I>
    requires game_of_life_cell_range<R, C, I>
    constexpr auto create_test_pattern_for_third_rule_2(const C& center_cell) -> TestPattern<R, C, I> {
        const auto initial = R{ center_cell, {center_cell.x + 1, center_cell.y} };
        const auto after_one_step = R{ };
        return { initial, after_one_step };
    }
    
    template <typename R, typename C, typename I>
    requires game_of_life_cell_range<R, C, I>
    constexpr auto create_test_pattern_for_third_rule_3(const C& center_cell) -> TestPattern<R, C, I> {
        const auto initial = R{ center_cell, {center_cell.x, center_cell.y + 1} };
        const auto after_one_step = R{ };
        return { initial, after_one_step };
    }

    template <typename R, typename C, typename I>
    requires game_of_life_cell_range<R, C, I>
    constexpr auto create_test_pattern_for_third_rule_4(const C& center_cell) -> TestPattern<R, C, I> {
        const auto initial = R{ center_cell, {center_cell.x + 1, center_cell.y + 1} };
        const auto after_one_step = R{ };
        return { initial, after_one_step };
    }



}

}