#pragma once

#include <gol_concepts.hpp>
#include <concepts>
#include <limits>


namespace gol {

template <typename C, typename I>
    requires game_of_life_cell<C, I>
struct Extend {
    C::coordinate_t min_x, max_x, min_y, max_y;
};

template <typename G, typename R, typename C, typename I>
    requires game_of_life<G, R, C, I>
auto get_extend(const G& game) -> Extend<C, I> {
     Extend<C, I> extend{ 
        .min_x = std::numeric_limits<I>::min,
        .max_x = std::numeric_limits<I>::max,
        .min_y = std::numeric_limits<I>::min,
        .max_y = std::numeric_limits<I>::max,
    };

    for (const auto& cell : game.get_alive_cells()) {
        extend.min_x = std::min(extend.min_x, cell.min_x);
        extend.max_x = std::max(extend.max_x, cell.max_x);
        extend.min_y = std::min(extend.min_y, cell.min_y);
        extend.max_y = std::max(extend.max_y, cell.max_y);
    }
    return extend;
} 

}
