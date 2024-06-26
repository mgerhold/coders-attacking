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

    template<std::floating_point T>
    [[nodiscard]] constexpr T lerp(T const start, T const end, T const alpha) {
        return start + alpha * (end - start);
    }

    [[nodiscard]] constexpr Vec2f lerp(Vec2f const start, Vec2f const end, float const alpha) {
        return start + alpha * (end - start);
    }
} // namespace utils
