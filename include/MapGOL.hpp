#pragma once
#include <gol_concepts.hpp>
#include <gol_utility.hpp>

#include <concepts>
#include <map>
#include <utility>
#include <algorithm>
#include <vector>
#include <functional>

#include <fmt/core.h>

namespace gol {

template <std::signed_integral I>
class MapGOL {
  public:
    using coordinate_t = I;
    using cell_t = NaturalCell<I>;

    using cell_range_t = std::vector<cell_t>;

  private:
    std::map<cell_t, bool> cells_;


  public:
    auto is_alive(const cell_t& cell) noexcept -> bool {
        return cells_[cell];
    };

    auto get_alive_cells() const -> cell_range_t {
        auto alive_cells = std::vector<cell_t>{ };
        for (const auto& [cell, is_alive] : cells_) {
            if (is_alive) {
                alive_cells.push_back(cell);
            }
        }
        return alive_cells;
    };

    auto next_step() -> void {
        auto amount_of_neighbors = std::map<cell_t, uint32_t>{ };

        for (const auto& [cell, alive] : cells_) {
            if (alive) {
                amount_of_neighbors[cell_t{cell.x + I { 1 }, cell.y + I { 1 }}] += 1;
                amount_of_neighbors[cell_t{cell.x + I { 1 }, cell.y}] += 1;
                amount_of_neighbors[cell_t{cell.x + I { 1 }, cell.y - I { 1 }}] += 1;
                amount_of_neighbors[cell_t{cell.x, cell.y + I { 1 }}] += 1;
                amount_of_neighbors[cell_t{cell.x, cell.y - I { 1 }}] += 1;
                amount_of_neighbors[cell_t{cell.x - I { 1 }, cell.y + I { 1 }}] += 1;
                amount_of_neighbors[cell_t{cell.x - I { 1 }, cell.y}] += 1;
                amount_of_neighbors[cell_t{cell.x - I { 1 }, cell.y - I { 1 }}] += 1;
            }
        }

        auto new_cells = std::map<cell_t, bool>{ };

        for (const auto& [cell, neighbors] : amount_of_neighbors) {
            if (cells_[cell]) {
                if (neighbors == 2 || neighbors == 3) {
                    new_cells[cell] = true;
                }
            } else {
                if (neighbors == 3) {
                    new_cells[cell] = true;
                }
            }
        }

        cells_ = std::move(new_cells);
    };  

    auto set_alive(const cell_t& cell) -> void {
        cells_[cell] = true;
    };

    auto set_alive(const cell_range_t& cells) -> void {
        for (const auto& cell : cells) {
            set_alive(cell);
        }
    };

    auto kill(const cell_t& cell) -> void {
        cells_[cell] = false;
    }

    auto kill(const cell_range_t& cells) -> void {
        for (const auto& cell : cells) {
            kill(cell);
        }
    }
};

static_assert(game_of_life<MapGOL<int>>);
static_assert(game_of_life<MapGOL<long int>>);

} // namespace gol