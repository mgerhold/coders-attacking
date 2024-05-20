#include <view/camera.hpp>

namespace view {
    using namespace utils;

    // clang-format off
    [[nodiscard]] Vec2f Camera::world_to_view_coords(
        Vec2f const world_coords,
        float const parallax_distance
    ) const { // clang-format on
        return world_coords * m_zoom - m_offset / parallax_distance + Vec2f{ 0.5f, 0.5f };
    }

    // clang-format off
    [[nodiscard]] Vec2i Camera::view_to_screen_coords(
        Vec2f const view_coords
    ) const { // clang-format on
        return Vec2i{ FloatRect{ m_viewport }.relative_to_absolute_position(view_coords) };
    }

    // clang-format off
    [[nodiscard]] Vec2i Camera::world_to_screen_coords(
        Vec2f const world_coords,
        float const parallax_distance
    ) const { // clang-format on
        return view_to_screen_coords(world_to_view_coords(world_coords, parallax_distance));
    }

    // clang-format off
    [[nodiscard]] Vec2f Camera::screen_to_view_coords(
        Vec2i const screen_coords
    ) const { // clang-format on
        return Vec2f{
            FloatRect{ m_viewport }.absolute_to_relative_position(Vec2f{ screen_coords }),
        };
    }

    Vec2f Camera::view_to_world_coords(Vec2f const view_coords) const {
        return (view_coords - Vec2f{ 0.5f, 0.5f } + m_offset) / m_zoom;
    }
} // namespace view
