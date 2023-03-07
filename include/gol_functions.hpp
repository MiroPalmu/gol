#pragma once


#include "gol_concepts.hpp"
#include "ansi.hpp"
#include <concepts>
#include <limits>
#include <vector>
#include <ranges>
#include <algorithm>
#include <fmt/core.h>
#include <optional>
#include <string>


namespace gol {

template <gol::game_of_life G>
struct Extent {
    G::coordinate_t min_x, max_x, min_y, max_y;

    auto contains(const typename G::cell_t& cell) const noexcept -> bool {
        return cell.x < max_x && cell.x >= min_x && cell.y >= min_y && cell.y < max_y;
    }
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
constexpr auto set_cells_alive_from_range(const R& cells, G& game) -> void {
    std::ranges::for_each(cells, [&](const auto& x) { game.set_alive(x); });
}

template <gol::game_of_life G>
struct PrintOptions {
    Extent<G> extent = {-20, 20, -20, 20};
    typename G::coordinate_t max_width = 40;
    typename G::coordinate_t max_height = 40;
    char alive_cell = 'X';
    char dead_cell = ' '; 
};

template<gol::game_of_life G>
auto print_game(const G& game, const PrintOptions<G>& print_options) -> void {

    const auto width = std::min(print_options.max_width, print_options.extent.max_x - print_options.extent.min_x);
    const auto height = std::min(print_options.max_height, print_options.extent.max_y - print_options.extent.min_y);
    const auto drawn_extent = Extent<G>{
        .min_x = print_options.extent.min_x,
        .max_x = print_options.extent.min_x + width,
        .min_y = print_options.extent.min_y,
        .max_y = print_options.extent.min_y + height
    };

    auto rows = std::vector<std::string>(height, std::string(width, print_options.dead_cell));
    const auto alive_cells = game.get_alive_cells();
    for (const auto cell : alive_cells) {
        if (drawn_extent.contains(cell)) {
            rows[cell.y - drawn_extent.min_y][cell.x - drawn_extent.max_x] = print_options.alive_cell;
        }
    }

    for (const auto& row : rows) {
        fmt::print("{}\n", row);
    }

    namespace ansi = srilakshmikanthanp::libansi;
    fmt::print("{}", ansi::str(ansi::cursorup(height)));
} 

template <gol::game_of_life G>
struct RunOptions {
    size_t max_iterations = 1000;
    PrintOptions<G> print_options = {};
};

template<gol::game_of_life G>
auto run_game(const RunOptions<G>& run_options) -> void {
    auto game = G{ };
    
    for (size_t iteration = 0; iteration < run_options.max_iterations; ++iteration) {
        print_game(game, run_options.print_options);

        game.next_step();
    }

}


}
