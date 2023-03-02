#pragma once


#include <gol_concepts.hpp>
#include <concepts>
#include <limits>


namespace gol {

template <gol::game_of_life G>
struct Extend {
    G::coordinate_t min_x, max_x, min_y, max_y;
};

template <gol::game_of_life G>
auto get_extend(const G& game) -> Extend<G> {
     Extend<G> extend{ 
        .min_x = std::numeric_limits<typename G::coordinate_t>::min,
        .max_x = std::numeric_limits<typename G::coordinate_t>::max,
        .min_y = std::numeric_limits<typename G::coordinate_t>::min,
        .max_y = std::numeric_limits<typename G::coordinate_t>::max,
    };

    for (const auto& cell : game.get_alive_cells()) {
        extend.min_x = std::min(extend.min_x, cell.min_x);
        extend.max_x = std::max(extend.max_x, cell.max_x);
        extend.min_y = std::min(extend.min_y, cell.min_y);
        extend.max_y = std::max(extend.max_y, cell.max_y);
    }
    return extend;
};


template <gol::game_of_life G>
void add_cells_from_plaintext_to_game(const std::string_view plaintext, G& game, const typename G::cell_t& where) {
    
    auto row_index = typename G::coordinate_t { 0 };
    for (const auto row : plaintext | std::views::split('\n')) {
        if (std::ranges::empty(row) || row.front() == '!') {
            continue;
        }

        auto column_index = typename G::coordinate_t { 0 };
        auto row_contained_info = false;
        for (const auto character : row) {
            if (character == 'O') {
                game.set_alive(typename G::cell_t { where.x + column_index, where.y + row_index});
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
}


}
