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

template <std::integral I>
class MapGOL {
  public:
    using coordinate_t = I;
    using cell_t = NaturalCell<I>;

    using cell_range_t = std::vector<cell_t>;

  private:
    std::map<cell_t, bool> cells;


  public:
    auto is_alive(const cell_t& cell) noexcept -> bool {
        return cells[cell];
    };

    auto get_alive_cells() const -> cell_range_t {
        auto alive_cells = std::vector<cell_t>{ };
        for (const auto& [key, is_alive] : cells) {
            if (is_alive) {
                alive_cells.push_back(key);
            }
        }
        return alive_cells;
    };

    auto next_step() -> void {
        fmt::print("Next step implementation missing!\n");
    };  

    auto set_alive(const cell_t& cell) -> void {
        cells[cell] = true;
    };

    auto set_alive(const cell_range_t& cells) -> void {
        for (const auto& cell : cells) {
            set_alive(cell);
        }
    };

    auto kill(const cell_t& cell) -> void {
        cells[cell] = false;
    }

    auto kill(const cell_range_t& cells) -> void {
        for (const auto& cell : cells) {
            kill(cell);
        }
    }
};

} // namespace gol