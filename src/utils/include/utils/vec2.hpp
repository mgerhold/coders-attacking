#pragma once

#include <concepts>
#include <stdexcept>

namespace utils {
    template<typename T>
    concept IsNumeric = std::integral<T> or std::floating_point<T>;

    template<IsNumeric T>
    class Vec2 final {
    public:
        T x{};
        T y{};

        constexpr Vec2() = default;
        constexpr Vec2(T const x, T const y) : x{ x }, y{ y } { }

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
