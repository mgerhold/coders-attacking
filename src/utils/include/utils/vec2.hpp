#pragma once

#include "concepts.hpp"
#include <stdexcept>

namespace utils {
    template<IsNumeric T>
    class Vec2 final {
    public:
        T x{};
        T y{};

        constexpr Vec2() = default;
        constexpr Vec2(T const x, T const y) : x{ x }, y{ y } { }

        template<IsNumeric U>
        constexpr explicit Vec2(Vec2<U> const other)
            requires(not std::same_as<T, U>)
            : x{ static_cast<T>(other.x) },
              y{ static_cast<T>(other.y) } { }

        constexpr Vec2& operator+=(Vec2 const& other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        constexpr Vec2& operator-=(Vec2 const& other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        constexpr Vec2& operator*=(T const& scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        constexpr Vec2& operator/=(T const& scalar) {
            if (scalar == T{}) {
                throw std::invalid_argument{ "division by zero" };
            }
            x /= scalar;
            y /= scalar;
            return *this;
        }

        constexpr Vec2 hadamard_product(Vec2 const other) const {
            return Vec2{ x * other.x, y * other.y };
        }

        [[nodiscard]] friend constexpr Vec2 operator+(Vec2 lhs, Vec2 const rhs) {
            lhs += rhs;
            return lhs;
        }

        [[nodiscard]] friend constexpr Vec2 operator-(Vec2 lhs, Vec2 const rhs) {
            lhs -= rhs;
            return lhs;
        }

        [[nodiscard]] friend constexpr Vec2 operator*(Vec2 lhs, T const rhs) {
            lhs *= rhs;
            return lhs;
        }

        [[nodiscard]] friend constexpr Vec2 operator*(T const lhs, Vec2 const rhs) {
            return rhs * lhs;
        }

        [[nodiscard]] friend constexpr Vec2 operator/(Vec2 lhs, T const rhs) {
            lhs /= rhs;
            return lhs;
        }

        [[nodiscard]] friend constexpr bool operator==(Vec2 const lhs, Vec2 const rhs) {
            return lhs.x == rhs.x and lhs.y == rhs.y;
        }
    };

    using Vec2f = Vec2<float>;
    using Vec2i = Vec2<int>;
} // namespace utils
