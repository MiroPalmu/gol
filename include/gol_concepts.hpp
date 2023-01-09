#pragma once

#include <concepts>
#include <ranges>

namespace gol {

template <typename C, typename I>
concept game_of_life_cell = std::integral<I> && requires (C cell, I coordinate) {
    { coordinate } -> std::same_as<typename C::coordinate_t>;

    { cell.x } -> std::same_as<I>;
    { cell.y } -> std::same_as<I>;

};

template <typename R, typename C, typename I>
concept game_of_life_cell_range = game_of_life_cell<C, I>
                                && std::ranges::range<R>
                                && std::movable<R>
                                && requires {
    { std::ranges::range_value_t<R>{ } } -> game_of_life_cell<I>;
};

template <typename G, typename R, typename C, typename I>
concept game_of_life = game_of_life_cell<C, I>
                    && game_of_life_cell_range<R, C, I>
                    && requires (G&& gol, R&& cell_range, C&& cell) {

    { cell } -> std::same_as<typename G::cell_t>;
    { cell_range } -> std::same_as<typename G::cell_range_t>;
    
    { gol.is_alive(cell) } -> std::same_as<bool>;
    { gol.get_alive_cells() } -> game_of_life_cell_range<C, I>;

    gol.next_step();
    gol.set_alive(cell);
    gol.set_alive(cell_range);
    gol.kill(cell);
    gol.kill(cell_range);
};

}