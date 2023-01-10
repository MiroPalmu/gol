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

}
