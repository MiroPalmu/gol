#pragma once

#include <concepts>
#include <ranges>
#include <initializer_list>

namespace gol {

template <typename C, typename I>
concept game_of_life_cell = std::signed_integral<I>
                        && std::equality_comparable<C>
                        && requires (C cell, I coordinate) {

    { C{ coordinate, coordinate } } -> std::same_as<C>;
    { cell.x } -> std::same_as<I&>;
    { cell.y } -> std::same_as<I&>;

};

// Lightweight but owning range for passing multiple cells at the same time
template <typename R, typename C, typename I>
concept game_of_life_cell_range = game_of_life_cell<C, I>
                                && std::ranges::range<R>
                                && std::movable<R>
                                && std::convertible_to<std::ranges::range_value_t<R>, C>
                                && requires (I coordinate, std::initializer_list<C> coordinates) {
    { R(coordinates) } -> std::convertible_to<R>;
    { R{ {coordinate, coordinate}, {coordinate, coordinate} } } -> std::convertible_to<R>;
};

template <typename G> 
concept game_of_life_type_aware = requires {
    typename G::cell_range_t;
    typename G::cell_t;
    typename G::coordinate_t;
};

template <typename G>
concept game_of_life = game_of_life_type_aware<G>
                    && game_of_life_cell<typename G::cell_t, typename G::coordinate_t>
                    && game_of_life_cell_range<typename G::cell_range_t, typename G::cell_t, typename G::coordinate_t>
                    && requires (G&& gol, typename G::cell_range_t&& cell_range, typename G::cell_t&& cell) {
    
    
    { gol.is_alive(cell) } -> std::same_as<bool>;
    { gol.get_alive_cells() } -> game_of_life_cell_range<typename G::cell_t, typename G::coordinate_t>;

    gol.next_step();
    gol.set_alive(cell);
    gol.set_alive(cell_range);
    gol.kill(cell);
    gol.kill(cell_range);
};

}