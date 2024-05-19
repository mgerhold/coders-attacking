#pragma once

#include "vec2.hpp"

namespace utils {
    template<IsNumeric T>
    struct Rect final {
        Vec2<T> top_left;
        Vec2<T> size;

        constexpr Rect() = default;
        constexpr Rect(Vec2<T> const top_left, Vec2<T> const size) : top_left{ top_left }, size{ size } { }
        constexpr Rect(T const x, T const y, T const width, T const height)
            : top_left{ x, y },
              size{ width, height } { }

        [[nodiscard]] static constexpr Rect unit() {
            return Rect{
                Vec2<T>{ T{ 0 }, T{ 0 } },
                Vec2<T>{ T{ 1 }, T{ 1 } },
            };
        }

        template<IsNumeric U>
        constexpr explicit Rect(Rect<U> const& other)
            requires(not std::same_as<T, U>)
            : top_left{ static_cast<T>(other.top_left.x), static_cast<T>(other.top_left.y) },
              size{ static_cast<T>(other.size.x), static_cast<T>(other.size.y) } { }

        [[nodiscard]] constexpr Vec2<T> relative_to_absolute_position(Vec2<T> const relative_position) const {
            return top_left + size.hadamard_product(relative_position);
        }

        [[nodiscard]] constexpr Rect relative_to_absolute_rect(Rect const& relative_rect) const {
            auto const absolute_top_left = relative_to_absolute_position(relative_rect.top_left);
            auto const absolute_bottom_right =
                    relative_to_absolute_position(relative_rect.top_left + relative_rect.size);
            auto const absolute_size = absolute_bottom_right - absolute_top_left;
            return Rect{ absolute_top_left, absolute_size };
        }
    };

    using IntRect = Rect<int>;
    using FloatRect = Rect<float>;
} // namespace utils
