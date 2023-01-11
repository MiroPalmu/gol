#include <gol_concepts.hpp>
#include <concepts>
#include <ranges>
#include <vector>
#include <algorithm>

namespace gol::testing {

    template <typename R, typename C, typename I>
    requires game_of_life_cell_range<R, C, I>
    auto ranges_contains_exactly_same_cells(const R& range_1, const R& range_2) -> bool {
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

    
}