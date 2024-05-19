#pragma once

#include <concepts>
#include <stdexcept>

namespace utils {
    template<std::unsigned_integral T>
    [[nodiscard]] constexpr T ceil_div(T const lhs, T const rhs) {
        if (rhs == T{}) {
            throw std::invalid_argument{ "division by zero" };
        }
        return T{ 1 } + ((lhs - T{ 1 }) / rhs);
    }
} // namespace utils
