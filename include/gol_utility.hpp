#pragma once

#include <concepts>
#include <gol_concepts.hpp>

namespace gol {
    
    template <std::integral I>
    struct NaturalCell {
        I x, y;
        auto operator<=>(const NaturalCell&) const = default;
    };
}