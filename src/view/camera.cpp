#include <view/camera.hpp>

namespace view {
    // clang-format off
    [[nodiscard]] utils::Vec2f Camera::world_to_view_coords(
        utils::Vec2f const world_coords,
        float const parallax_distance
    ) const { // clang-format on
        return world_coords * m_zoom - m_offset / parallax_distance + utils::Vec2f{ 0.5f, 0.5f };
    }

    [[nodiscard]] utils::FloatRect Camera::world_to_view_coords(utils::FloatRect const world_coords) const {
        auto const top_left = world_to_view_coords(world_coords.top_left);
        auto const bottom_right = world_to_view_coords(world_coords.top_left + world_coords.size);
        return utils::FloatRect{ top_left, bottom_right - top_left };
    }

    // clang-format off
    [[nodiscard]] utils::Vec2i Camera::view_to_screen_coords(
        utils::Vec2f const view_coords,
        utils::IntRect const viewport
    ) const { // clang-format on
        return utils::Vec2i{ utils::FloatRect{ viewport }.relative_to_absolute_position(view_coords) };
    }

    // clang-format off
    [[nodiscard]] utils::IntRect Camera::view_to_screen_coords(
        utils::FloatRect const view_coords,
        utils::IntRect const viewport
    ) const { // clang-format on
        return utils::IntRect{ utils::FloatRect{ viewport }.relative_to_absolute_rect(view_coords) };
    }

    // clang-format off
    [[nodiscard]] utils::Vec2i Camera::world_to_screen_coords(
        utils::Vec2f const world_coords,
        utils::IntRect const viewport,
        float const parallax_distance
    ) const { // clang-format on
        return view_to_screen_coords(world_to_view_coords(world_coords, parallax_distance), viewport);
    }

    // clang-format off
    [[nodiscard]] utils::IntRect Camera::world_to_screen_coords(
        utils::FloatRect const world_coords,
        utils::IntRect const viewport
    ) const { // clang-format on
        return view_to_screen_coords(world_to_view_coords(world_coords), viewport);
    }
} // namespace view
