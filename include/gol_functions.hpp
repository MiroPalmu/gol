#pragma once


#include <gol_concepts.hpp>
#include <concepts>
#include <limits>
#include <vector>
#include <ranges>
#include <algorithm>
#include <fmt/core.h>


namespace gol {

template <gol::game_of_life G>
struct Extent {
    G::coordinate_t min_x, max_x, min_y, max_y;
};


template <gol::game_of_life G>
auto get_extent(const typename G::cell_range_t& alive_cells) -> Extent<G> {
     Extent<G> extent{ 
        .min_x = std::numeric_limits<typename G::coordinate_t>::max(),
        .max_x = std::numeric_limits<typename G::coordinate_t>::min(),
        .min_y = std::numeric_limits<typename G::coordinate_t>::max(),
        .max_y = std::numeric_limits<typename G::coordinate_t>::min(),
    };

    if (std::ranges::empty(alive_cells)) {
        return { };
    }
    for (const auto& cell : alive_cells) {
        extent.min_x = std::min(extent.min_x, cell.x);
        extent.max_x = std::max(extent.max_x, cell.x);
        extent.min_y = std::min(extent.min_y, cell.y);
        extent.max_y = std::max(extent.max_y, cell.y);
    }
    return extent;
};



template <gol::game_of_life G>
auto get_extent(const G& game) -> Extent<G> {

    const auto alive_cells = game.get_alive_cells();
    return get_extent<G>(alive_cells);
};



template <gol::game_of_life G>
constexpr auto parse_cells_from_plaintext(const std::string_view plaintext, const typename G::cell_t where) -> std::vector<typename G::cell_t> {
    auto cells = std::vector<typename G::cell_t>{ };    

    auto row_index = typename G::coordinate_t { 0 };
    for (const auto row : plaintext | std::views::split('\n')) {
        if (std::ranges::empty(row) || row.front() == '!') {
            continue;
        }

        auto column_index = typename G::coordinate_t { 0 };
        auto row_contained_info = false;
        for (const auto character : row) {
            if (character == 'O') {
                cells.push_back(typename G::cell_t { where.x + column_index, where.y + row_index});
                ++column_index;
                row_contained_info = true;
            } else if (character == '.') {
                ++column_index;
                row_contained_info = true;
            }
        }
        
        if (row_contained_info) {
            ++row_index;
        }
    }

    return cells;
}

template <std::ranges::range R, gol::game_of_life G>
requires std::convertible_to<std::ranges::range_value_t<R>, typename G::cell_t> 
constexpr auto set_cells_from_range(const R& cells, G& game) -> void {
    std::ranges::for_each(cells, [&](const auto& x) { game.set_alive(x); });
}


template<gol::game_of_life G>
auto print_game(const G& game) -> void {

}


}
